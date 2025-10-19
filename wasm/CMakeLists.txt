cmake_minimum_required(VERSION 3.5)
project(sl-wasm C)

set(CMAKE_C_STANDARD 11)

# Default WasmEdge build dir (same as original build.sh). Can be overridden with -DWASMEDGE_DIR=...
if(NOT DEFINED WASMEDGE_DIR)
  set(WASMEDGE_DIR "${CMAKE_SOURCE_DIR}/../funera1-WasmEdge/build")


set(WASMEDGE_INCLUDE "${WASMEDGE_DIR}/include/api" CACHE PATH "WasmEdge include directory")
set(Wendif()ASMEDGE_LIBDIR "${WASMEDGE_DIR}/lib/api" CACHE PATH "WasmEdge library directory")

message(STATUS "WASMEDGE_DIR = ${WASMEDGE_DIR}")
message(STATUS "WASMEDGE_INCLUDE = ${WASMEDGE_INCLUDE}")
message(STATUS "WASMEDGE_LIBDIR = ${WASMEDGE_LIBDIR}")

if(NOT EXISTS "${WASMEDGE_DIR}")
  message(WARNING "WASMEDGE_DIR does not exist: ${WASMEDGE_DIR}")
endif()

# Sources
set(SRC_FILES
  ${CMAKE_SOURCE_DIR}/main.c
)

add_executable(main ${SRC_FILES})

target_include_directories(main PRIVATE ${WASMEDGE_INCLUDE})

# Prefer explicit library file if found in WASMEDGE_LIBDIR, otherwise let CMake/Linker find 'wasmedge'
find_library(WASMEDGE_LIB_NAMED
  NAMES wasmedge
  PATHS ${WASMEDGE_LIBDIR}
  NO_DEFAULT_PATH
)

if(WASMEDGE_LIB_NAMED)
  message(STATUS "Found WasmEdge library: ${WASMEDGE_LIB_NAMED}")
  target_link_libraries(main PRIVATE ${WASMEDGE_LIB_NAMED})
else()
  message(STATUS "Falling back to linking 'wasmedge' (system paths). If link fails, set WASMEDGE_LIBDIR or WASMEDGE_DIR correctly.")
  target_link_libraries(main PRIVATE wasmedge)
endif()

# Ensure debug symbols by default (similar to -g in build.sh)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

target_compile_options(main PRIVATE -g)

# Set rpath so the built binary can find libwasmedge at runtime when pointed to WASMEDGE_LIBDIR
set_target_properties(main PROPERTIES
  BUILD_RPATH "${WASMEDGE_LIBDIR}"
)

install(TARGETS main RUNTIME DESTINATION bin)
