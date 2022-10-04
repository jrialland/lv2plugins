
from .model import Plugin, PluginType, generate_audio_plugin_desc, generate_manifest
from .metacomments import scan_directory
import sys
import json
import os

if __name__ == '__main__':
    builddir = sys.argv[1]
    for uri, meta in scan_directory('../src').items():
        plugin = Plugin.from_json(json.dumps(meta))  # type:ignore
        targetdir = os.path.join(builddir, f'{plugin.name}.lv2')
        if not os.path.isdir(targetdir):
            os.makedirs(targetdir)
        with open(os.path.join(targetdir, 'manifest.ttl'), 'wb') as f:
            g = generate_manifest(plugin)
            g.serialize(destination=f, format='turtle')

        if plugin.pluginType is PluginType.AUDIO:
            with open(os.path.join(targetdir, f'{plugin.name}.ttl'), 'wb') as f:
                g = generate_audio_plugin_desc(plugin)
                g.serialize(destination=f, format='turtle')
