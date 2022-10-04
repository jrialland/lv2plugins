#!/bin/bash
set -euo pipefail
thisscript=$(readlink -m "$0")
thisdir=$(dirname "$thisscript")
pushd "$thisdir" >/dev/null
trap "popd" EXIT
if [ ! -d ./venv ]; then
    python3 -m venv venv
    source venv/bin/activate
    pip install rdflib dataclasses-json
else
    source venv/bin/activate
fi
python3 -m lv2manifest $@
