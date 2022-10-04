#!/bin/bash
set -euo pipefail
thisscript=$(readlink -m "$0")
thisdir=$(dirname "$thisscript")
pushd "$thisdir" >/dev/null
if [ ! -d ./venv ]; then
    python -m venv venv
    source venv/bin/activate
    pip install dataclasses-json ordf
else
    source venv/bin/activate
fi
python -m lv2manifest $@
popd
