cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 20)
file(REMOVE_RECURSE build)

# MSVC和GNU之外的其他编译器我没有，所以没试，暂时设成较大的32
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_definitions(-DPOINTER_BUFFER_SIZE=8)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_definitions(-DPOINTER_BUFFER_SIZE=24)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_definitions(-DPOINTER_BUFFER_SIZE=24)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
    add_definitions(-DPOINTER_BUFFER_SIZE=24)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
    add_definitions(-DPOINTER_BUFFER_SIZE=24)
else()
    add_definitions(-DPOINTER_BUFFER_SIZE=24)
endif()

project(SigSlot)
add_subdirectory(test)
