WASMEDGE_DIR=$(pwd)/../funera1-WasmEdge/build
WASMEDGE_LIB=$WASMEDGE_DIR/lib/api
WASMEDGE_INCLUDE=$WASMEDGE_DIR/include/api

local_mode=0
restore_mode=0

# export C_INCLUDE_PATH=$WASMEDGE_INCLUDE:$C_INCLUDE_PATH
export CPATH=$WASMEDGE_INCLUDE
gcc main.c \
  -g                  \
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
