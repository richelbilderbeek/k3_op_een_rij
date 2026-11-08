# Building

K3 Op Een Rij uses Qt6 and has no other dependencies.
The desktop version plays music, using Qt Multimedia.

## Desktop

On a terminal, do:

```bash
sudo apt-get install qt6-base-dev qt6-multimedia-dev gstreamer1.0-libav
git clone https://github.com/richelbilderbeek/k3_op_een_rij.git
cd k3_op_een_rij
mkdir build
cd build
qmake6 ../k3_op_een_rij.pro
make
./k3_op_een_rij
```

## Tests

```bash
mkdir build_test
cd build_test
qmake6 CONFIG+=debug ../k3_op_een_rij_test.pro
make
./k3_op_een_rij_test
```

## Web version

The web version uses [Qt for WebAssembly](https://doc.qt.io/qt-6/wasm.html),
which needs a matching version of [Emscripten](https://emscripten.org).
See the build workflow, `.github/workflows/build.yaml`,
for the versions used.

```bash
mkdir build_wasm
cd build_wasm
~/Qt/6.11.1/wasm_singlethread/bin/qmake ../k3_op_een_rij.pro
make
python3 -m http.server
```

Then open `http://localhost:8000/k3_op_een_rij.html` in a web browser.

## Continuous integration

On every push, GitHub Actions builds the Linux, Windows and web versions.
These can be downloaded as artifacts of the 'Build' workflow.
The website, including the playable web version,
is built by the 'Create Zensical website' workflow.
