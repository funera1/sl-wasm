# Build with CMake

This folder contains a simple CMake build for the `main` program which links against a local WasmEdge build (same behavior as the original `build.sh`).

Usage examples (from project `wasm` directory):

1) Out-of-source build (recommended) with default WASMEDGE_DIR:

   mkdir -p build && cd build
   cmake ..
   cmake --build . --config Debug

2) Specify a custom WasmEdge build directory (overrides default):

   mkdir -p build && cd build
   cmake -DWASMEDGE_DIR=/path/to/wasmedge/build ..
   cmake --build . --config Debug

The CMake target `main` will be built and its rpath set to the WasmEdge lib directory so you can run it directly:

   ./main

If the loader can't find libwasmedge, adjust `WASMEDGE_DIR` to point to the directory that contains `include/api` and `lib/api`.
