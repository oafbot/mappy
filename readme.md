Compile:
-------------

emcc main.cpp -o index.html --shell-file shell.html --emrun -s WASM=1 -s USE_SDL=2 -O3