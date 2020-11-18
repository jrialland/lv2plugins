#!/usr/bin/env python3
import os, sys, re, glob, logging
import json


re_declare = re.compile(r'DECLARE_PLUGIN\([ \t]*"(.+)",[ \t]*(.+)[ \t]*\);?')
re_plugin_description = re.compile(r'PLUGIN_DESCRIPTION\([ \t]*"(.+)"[ \t]*,[ \t]*"(.+)"[ \t]*\);?')
re_plugin_add_port = re.compile(r'PLUGIN_ADD_PORT\([ \t]*"(.+)"[ \t]*,[ \t]*([0-9]+)[ \t]*,[ \t]*"(.+)"[ \t]*,[ \t]*"(.+)"[ \t]*\)')

def lib_ext(base):
        """for a given base name, returns the name for a library file"""
        import platform
        s = platform.system()
        if s == 'Windows' or s.startswith('CYGWIN'):
                return base + '.dll'
        return 'lib' + base + '.so'

def get_shortname(uri):
        """determine the 'short name' of a plugin using its uri"""
        return re.sub('#', '_', uri.split('/')[-1])


def scan_infos(dir='../src'):
        """ read c++ code, parsing the macros that determine the plugin's metadata"""
        infos = {}
        for filename in glob.glob(dir + '/**/*.hpp'):
                with open(filename) as f:
                        for line in f:
                                line = line.strip()
                                if m:= re_declare.match(line):
                                        infos[m.group(1)] = {'class':m.group(2), 'ports':[], 'description':''}
                                elif m:= re_plugin_description.match(line):
                                        infos[m.group(1)]['description'] = m.group(2)
                                elif m:= re_plugin_add_port.match(line):
                                        name = m.group(4)
                                        infos[m.group(1)]['ports'].append({'id':int(m.group(2)), 'type':m.group(3), 'name':name})
                                        infos[m.group(1)]['ports'][-1]['symbol'] = name.lower()
        for uri, inf in infos.items():
                inf['plugin_uri'] = uri
                inf['plugin_shortname'] = get_shortname(uri)
                inf['dll_name'] = lib_ext(inf['plugin_shortname'])

        return infos



def manifest_ttl(config, filename):
        with open(filename, 'w') as f:
              f.write("""
@prefix lv2:  <http://lv2plug.in/ns/lv2core#> .
@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .
<{plugin_uri}>
        a lv2:Plugin ;
        lv2:binary <{dll_name}>  ;
        rdfs:seeAlso <{plugin_shortname}.ttl> .
""".format(**config).strip())

def port_decl(port):
        port['lv2_type'] = {'AUDIO_IN':'a lv2:AudioPort, lv2:InputPort', 'AUDIO_OUT':'a lv2:AudioPort, lv2:OutputPort'}[port['type']]
        return """
[
  {lv2_type} ;
  lv2:index {id} ;
  lv2:symbol "{symbol}" ;
  lv2:name "{name}"
]""".format(**port)

def plugin_ttl(config, filename):
        ttl = """
@prefix doap:  <http://usefulinc.com/ns/doap#> .
@prefix lv2:   <http://lv2plug.in/ns/lv2core#> .
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

<{plugin_uri}>
        a lv2:Plugin ;
        a doap:Project ;
        doap:name "{description}" ;
        doap:license <https://opensource.org/licenses/ISC> ;
        doap:maintainer <https://github.com/jrialland#me> ;
        lv2:optionalFeature lv2:hardRTCapable ;
""".format(**config).strip()
        if config['ports']:
                ttl += '\nlv2:port'
                ttl += ', '.join(map(port_decl, config['ports']))
        ttl += ' .'

        with open(filename, 'w') as f:
                f.write(ttl)

def write_manifests(inf, output = '.'):
        outdir = os.path.join(output, inf['plugin_shortname'] + '.lv2')
        if not os.path.isdir(outdir):
                os.makedirs(outdir)
        manifest_ttl(inf, os.path.join(outdir, 'manifest.ttl'))
        plugin_ttl(inf, os.path.join(outdir, inf['plugin_shortname']+'.ttl'))

################################################################################
if __name__ == '__main__':
        logging.basicConfig(level=logging.DEBUG)
        outdir = sys.argv[-1] if len(sys.argv) > 1 else '.'
        infos = scan_infos()
        for uri, inf in infos.items():
                logging.debug(json.dumps(infos, indent=4, sort_keys=True))
                write_manifests(inf, outdir)
        
