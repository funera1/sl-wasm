WASMEDGE_DIR=$(pwd)/../funera1-WasmEdge/build
WASMEDGE_LIB=$WASMEDGE_DIR/lib/api
WASMEDGE_INCLUDE=$WASMEDGE_DIR/include/api

arch=$(uname -m)

if [ $arch = "x86_64" ]; then
  is_x64=1
  echo "This is a x86_64 system."
else
  is_x64=0
  echo "Unknown architecture: $arch"
fi

# export C_INCLUDE_PATH=$WASMEDGE_INCLUDE:$C_INCLUDE_PATH
export CPATH=$WASMEDGE_INCLUDE
gcc main.c \
  -g                  \
  -DX64=$is_x64       \
  -o run-sl-wasm      \
  -I$WASMEDGE_INCLUDE \
  -L$WASMEDGE_LIB     \
  -lwasmedge          \
  -Wl,-rpath, $WASMEDGE_LIB/libwasmedge.so


# buildが失敗
if [ $? -eq 1 ]; then
  echo "Failed to build"
  exit 1
fi
