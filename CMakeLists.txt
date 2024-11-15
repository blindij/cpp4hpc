cmake_minimum_required(VERSION 3.28)
project(cpp4hpc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CheckCXXCompilerFlag)

set(_CXX_FLAGS)
if (_march_native_works)
    message(STATUS "Using processor's vector instructions (-march=native compiler flag set)")
    set(_CXX_FLAGS "-march=native")
elseif(_xhost_works)
    message(STATUS "Using processor's vector instructions (-xHost compiler flag set)")
    set(_CXX_FLAGS "-xHost")
else ()
    message(STATUS "No suitable compiler flag found for vectorization. We are setting -O3")
    set(_CXX_FLAGS "-O3")
endif()

add_executable(hw_unoptimized main.cpp)
add_executable(hw main.cpp)
target_compile_options(hw
  PRIVATE
  ${_CXX_FLAGS}
)
