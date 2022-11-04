#!/usr/bin/env python3
from enum import Enum, auto
from io import BytesIO, TextIOWrapper
from typing import List, Type, TypeVar, Union
from dataclasses import dataclass, field
from dataclasses_json import dataclass_json
from marshmallow import fields

from rdflib import Graph, Namespace, URIRef, Literal, IdentifiedNode, BNode
from rdflib.namespace import RDF, RDFS, DOAP

FOAF = Namespace("http://xmlns.com/foaf/0.1/")
lv2 = Namespace('http://lv2plug.in/ns/lv2core#')
lv2midi = Namespace('http://lv2plug.in/ns/ext/midi#')
atom = Namespace('http://lv2plug.in/ns/ext/atom#')


T = TypeVar('T', bound=Enum)
def string_enum(cls: Type[T]) -> Type[T]:
    class EnumField(fields.Field):
        def _serialize(self, value, attr, obj, **kwargs):
            return value.name

        def _deserialize(self, value, attr, data, **kwargs):
            return cls[value]
    if (not hasattr(cls, '__metadata__')):
        setattr(cls, '__metadata__', dict())
    metadata = {
        "dataclasses_json": {
            "encoder": lambda v: v.name,
            "decoder": lambda name: cls[name],
            "mm_field": EnumField(),
        }
    }
    getattr(cls, '__metadata__').update(metadata)
    return cls


@string_enum
class PortType(Enum):
    AUDIO_IN = 1
    AUDIO_OUT = 2
    MIDI_IN = 3
    MIDI_OUT = 4
    CONTROL_IN = 5
    CONTROL_OUT = 6


@string_enum
class PluginType(Enum):
    AUDIO = auto()
    UI = auto()


@dataclass_json
@dataclass
class Port:
    portType: PortType = field(metadata=getattr(PortType, '__metadata__'))
    symbol: str
    name: str
    minimum:float=.0
    default:float=.0
    maximum:float=.0

def set_maintainer(root: Graph, subject: URIRef, name: str, homepage: Union[str, None] = None, mbox: Union[str, None] = None):
    b = BNode()
    root.add((b, FOAF.name, Literal(name)))
    if homepage:
        root.add((b, FOAF.homepage, URIRef(homepage)))
    if mbox:
        root.add((b, FOAF.mbox, URIRef(f'mailto:{mbox}')))
    root.add((subject, DOAP.maintainer, b))


@dataclass_json
@dataclass
class Plugin:
    uri: str
    name: str
    pluginType: PluginType = field(
        metadata=getattr(PluginType, '__metadata__'))
    description: str
    ports: List[Port]
    ui: str

    def has_only_one(self, ofType:PortType)->bool:
        c = 0
        for port in self.ports:
            if port.portType is ofType:
                c += 1
            if c > 1:
                return False
        return c == 1

def generate_audio_plugin_desc(plugin: Plugin) -> Graph:
    g = Graph()
    g.bind('lv2', lv2)
    g.bind('doap', DOAP)
    g.bind("foaf", FOAF)
    pluginuri = URIRef(plugin.uri)
    set_maintainer(g, pluginuri, "Julien Rialland",
                   "https://www.github.com/jrialland", "julien.rialland@gmail.com")
    g.add((pluginuri, RDF.type, lv2.Plugin))
    g.add((pluginuri, RDF.type, lv2.AudioPlugin))
    g.add((pluginuri, RDF.type, DOAP.Project))
    g.add((pluginuri, DOAP.name, Literal(plugin.name)))
    g.add((pluginuri, lv2.binary, URIRef(lib_ext(plugin.name))))
    g.add((pluginuri, lv2.description, Literal(plugin.description, lang="en")))
    g.add((pluginuri, lv2.requiredFeature, URIRef("http://lv2plug.in/ns/ext/urid#map")))
    g.add((pluginuri, lv2.optionalFeature, lv2.hardRTCapable))
    for i, port in enumerate(plugin.ports):
        portNode = BNode()
        g.add((portNode, lv2.name, Literal(port.name)))
        g.add((portNode, lv2.symbol, Literal(port.symbol)))
        g.add((portNode, lv2['index'], Literal(i))),  # type:ignore
        g.add((pluginuri, lv2.port, portNode))
        if port.portType in [PortType.AUDIO_IN, PortType.MIDI_IN, PortType.CONTROL_IN]:
            g.add((portNode, RDF.type, lv2.InputPort))
        else:
            g.add((portNode, RDF.type, lv2.OutputPort))
        if port.portType in [PortType.AUDIO_IN, PortType.AUDIO_OUT]:
            g.add((portNode, RDF.type, lv2.AudioPort))
        elif port.portType in [PortType.MIDI_IN, PortType.MIDI_OUT]:
            g.add((portNode, RDF.type, atom.AtomPort))
            g.add((portNode, atom.bufferType, atom.Sequence))
            g.add((portNode, atom.supports, lv2midi.MidiEvent))

        if port.portType is PortType.CONTROL_IN:
            assert port.minimum <= port.default <= port.maximum
            g.add((portNode, RDF.type, lv2.ControlPort))
            g.add((portNode, lv2.default, Literal(port.default)))
            g.add((portNode, lv2.minimum, Literal(port.minimum)))
            g.add((portNode, lv2.maximum, Literal(port.maximum)))
        if port.portType is PortType.MIDI_IN and plugin.has_only_one(PortType.MIDI_IN):
            g.add((portNode, lv2.designation, lv2.control)) # This is necessary since it is possible to have several MIDI input ports, though typically it is best to have one.
    return g


def lib_ext(base):
    """for a given base name, returns the name for a library file"""
    import platform
    s = platform.system()
    if s == 'Windows' or s.startswith('CYGWIN'):
        return base + '.dll'
    return 'lib' + base + '.so'


def generate_manifest(plugin: Plugin) -> Graph:
    g = Graph()
    g.bind('lv2', lv2)
    g.bind('rdfs', RDFS)
    uri = URIRef(plugin.uri)
    g.add((uri, RDF.type, lv2.Plugin))
    g.add((uri, lv2.binary, URIRef(lib_ext(plugin.name))))
    g.add((uri, RDFS.seeAlso, URIRef(f'{plugin.name}.ttl')))
    return g
