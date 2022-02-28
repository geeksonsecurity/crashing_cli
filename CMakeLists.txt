cmake_minimum_required(VERSION 3.21)
project(crashing_cli)

set(CMAKE_CXX_STANDARD 14)

# Register Zydis dependency.
# Disable build of tools and examples.
option(ZYDIS_BUILD_TOOLS "" OFF)
option(ZYDIS_BUILD_EXAMPLES "" OFF)
add_subdirectory("deps/zydis")

add_executable(crashing_cli main.cpp)

#disable optimizations
target_compile_options(crashing_cli PRIVATE "/Ob0")
target_compile_options(crashing_cli PRIVATE "/Od")

# Have CMake link our project executable against Zydis.
target_link_libraries(crashing_cli PRIVATE "Zydis")