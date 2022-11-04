#!/usr/bin/env bash
set -euo pipefail

if [ ! -d build ]; then
    mkdir build
fi

pushd build
cmake ..
cmake --build .
./tests

mkdir -p "$HOME/.lv2"

for d in $(find ./ -type d -name "*.lv2"); do
    cp -R "$d" "$HOME/.lv2" 
done

for uri in $(lv2file --list | grep "jrialland/lv2plugins" | awk '{print $2}'); do
    echo '------------------- ' $uri '------------------- '
    lv2file -n $uri
done