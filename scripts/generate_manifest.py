#!/usr/bin/env python3
import os, sys, re, glob, logging
import json
from urllib.parse import urlparse


ccomment = re.compile(r'''(?x)
(?=["'/])      # trick to make it faster, a kind of anchor
(?:
    "(?=((?:[^"\\?]+|\?(?!\?/)|(?:\?\?/|\\)[\s\S])*))\1" # double quotes string
  |
    '(?=((?:[^'\\?]+|\?(?!\?/)|(?:\?\?/|\\)[\s\S])*))\2' # single quotes string
  |
    (
        /(?:(?:\?\?/|\\)\n)*/(?:.*(?:\?\?|\\)/\n)*.* # single line comment
      |
        /(?:(?:\?\?/|\\)\n)*\*                       # multiline comment
        (?=((?:[^*]+|\*+(?!(?:(?:\?\?/|\\)\n)*/))*))\4
        \*(?:(?:\?\?/|\\)\n)*/             
    )
)
''')

def lib_ext(base):
        """for a given base name, returns the name for a library file"""
        import platform
        s = platform.system()
        if s == 'Windows' or s.startswith('CYGWIN'):
                return base + '.dll'
        return 'lib' + base + '.so'

def get_metacomment(filename):
        comments = [m[2].strip() for m in ccomment.findall(open(filename).read()) if m[2]]
        comments = [s for c in comments for s in c.split('\n')]
        comments = ''.join([re.sub('^\*','',s.strip()) for s in filter(lambda s: not(s.startswith('//') or s.startswith('/*')), comments)])
        meta = re.search('@lv2cpp\.meta {(.+)}', comments)
        return json.loads('{' + meta[1] + '}') if meta else None

def scan_infos(dir='../src'):
        """ read c++ code, parsing the macros that determine the plugin's metadata"""
        infos = {}
        for filename in glob.glob(dir + '/**/*.hpp'):
                meta = get_metacomment(filename)
                if meta:
                        meta['__file__'] = filename
                        uri = meta['uri']
                        infos[uri] = meta
                        if not 'shortname' in meta:
                                s = os.path.basename(urlparse(meta['uri'].replace('#','-')).path)
                                meta['shortname'] = s
                        if not 'dll_name' in meta:
                                meta['dll_name'] = lib_ext(meta['shortname'])
                        logging.debug(meta)
        return infos

def emit_manifest_ttl(config, filename):
        with open(filename, 'w') as f:

                if config['plugin_type'] == 'audio' :
                        f.write("@prefix lv2:  <http://lv2plug.in/ns/lv2core#> .\n")
                        f.write("@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .\n")
                        f.write(f"<{config['uri']}>\n")
                        f.write(f"      a lv2:Plugin ;\n")
                        f.write(f"      lv2:binary <{config['dll_name']}>  ;\n")
                        f.write(f"      rdfs:seeAlso <{config['shortname']}.ttl> .")
                        
                elif config['plugin_type'] == 'ui':
                        f.write("@prefix lv2ui: <http://lv2plug.in/ns/extensions/ui#>.\n")
                        f.write(f"<{config['uri']}>\n")
                        f.write(f"      a lv2ui:Qt5UI ;\n")
                        f.write(f"      lv2ui:binary <{config['dll_name']}>  .")
                        

def port_decl(index, port):
        port['lv2_type'] = {
                'AUDIO_IN':'a lv2:AudioPort, lv2:InputPort',
                'AUDIO_OUT':'a lv2:AudioPort, lv2:OutputPort',
                'MIDI_IN':'a lv2ev:EventPort, lv2:InputPort',
                'MIDI_OUT':'a lv2ev:EventPort, lv2:OutputPort'
        }[port['type']]
        decl = '[\n'
        decl += f"  {port['lv2_type']} ;\n"
        decl += f"  lv2:index {index} ;\n"
        decl += f'''  lv2:symbol "{port['symbol']}" ;\n'''
        decl += f'''  lv2:name "{port['name']}" ;\n'''
        if port['lv2_type'] in ['MIDI_IN', 'MIDI_OUT']:
                decl += '  lv2ev:supportsEvent <http://lv2plug.in/ns/ext/midi#MidiEvent> ;\n'
        decl+=']'
        return decl

def emit_audio_ttl(config, filename):
        ttl = """
@prefix doap:  <http://usefulinc.com/ns/doap#> .
@prefix lv2:   <http://lv2plug.in/ns/lv2core#> .
@prefix lv2ev: <http://lv2plug.in/ns/ext/event#>.
@prefix lv2ui: <http://lv2plug.in/ns/extensions/ui#> .
@prefix rdf:   <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix rdfs:  <http://www.w3.org/2000/01/rdf-schema#> .
@prefix units: <http://lv2plug.in/ns/extensions/units#> .
@prefix foaf:  <http://xmlns.com/foaf/0.1/> .

<https://github.com/jrialland#me>
        a foaf:Person ;
        foaf:name "Julien Rialland" ;
        foaf:mbox <mailto:julien.rialland@gmail.com> ;
        foaf:homepage <https://github.com/jrialland> .

<{uri}>
        a lv2:Plugin ;
        a doap:Project ;
        doap:name "{description}" ;
        doap:license <https://opensource.org/licenses/ISC> ;
        doap:maintainer <https://github.com/jrialland#me> ;
        lv2:optionalFeature lv2:hardRTCapable ;
""".format(**config).strip()
        if 'ui' in config:
                ttl += f'''\n        lv2ui:ui <{config['ui']}> ;\n'''
        if 'ports' in config:
                ttl += '\nlv2:port'
                ports = [port_decl(i, p) for i,p in enumerate(config['ports'])]
                ttl += ', '.join(ports)
        ttl += ' .'
        with open(filename, 'w') as f:
                logging.info(filename)
                f.write(ttl)

################################################################################
if __name__ == '__main__':
        logging.basicConfig(level=logging.DEBUG)
        if len(sys.argv) < 2:
                raise Exception("usage : <script> <outputdir>")
        for uri, info in scan_infos().items():
                outputdir = os.path.join(sys.argv[-1], info['shortname'] + '.lv2')
                if not os.path.isdir(outputdir):
                        os.makedirs(outputdir)
                emit_manifest_ttl(info, os.path.join(outputdir, 'manifest.ttl'))
                if info['plugin_type'] == 'audio':
                        emit_audio_ttl(info, os.path.join(outputdir, info['shortname'] + '.ttl'))