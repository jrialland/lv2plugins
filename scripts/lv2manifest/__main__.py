from .model import Plugin, PluginType, generate_audio_plugin_desc, generate_manifest
from .metacomments import scan_directory
import sys
import json
import os
from .logging import configure_logging
import logging

if __name__ == "__main__":
    configure_logging()
    if len(sys.argv) != 2:
        builddir = os.path.realpath(
            os.path.join(os.path.dirname(__file__), "..", "..", "build")
        )
    else:
        builddir = sys.argv[1]
    logging.debug(f"builddir : {builddir}")
    sourcedir = os.path.realpath(os.path.join(builddir, "..", "src"))
    logging.debug(f"sourcedir : {sourcedir}")
    found_annotations = scan_directory(sourcedir)
    if not found_annotations:
        logging.warning("0 annotations were found")

    # for every plugin detected in the sources
    for uri, meta in found_annotations.items():
        # parse the json in order to have an object
        plugin = Plugin.from_json(json.dumps(meta))  # type:ignore
        logging.info(f"Detected Plugin : {plugin.name} ( {plugin.uri} )")
        # where the files will be generated
        targetdir = os.path.join(builddir, f"{plugin.name}.lv2")
        if not os.path.isdir(targetdir):
            os.makedirs(targetdir)

        # write a "general" manifest file

        manifestttl = os.path.join(targetdir, "manifest.ttl")
        logging.info(f"  - writing {manifestttl}")
        with open(manifestttl, "wb") as f:
            g = generate_manifest(plugin)
            g.serialize(destination=f, format="turtle")

        # for audio plugins

        if plugin.pluginType is PluginType.AUDIO:
            pluginttl = os.path.join(targetdir, f"{plugin.name}.ttl")
            logging.info(f"  - writing {pluginttl}")
            with open(pluginttl, "wb") as f:
                g = generate_audio_plugin_desc(plugin)
                g.serialize(destination=f, format="turtle")
