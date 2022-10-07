if [ ! -d build ]; then
    mkdir build
fi

pushd build
cmake ..
cmake --build .

mkdir -p "$HOME/.lv2"

for d in $(find ./ -type d -name "*.lv2"); do
    cp -R "$d" "$HOME/.lv2" 
done

lv2file --list | grep "jrialland/lv2plugins"