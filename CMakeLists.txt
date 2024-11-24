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
elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
    message(STATUS "Using Apple M1 vector instructions (-mcpu=apple-m1 compiler flag set)")
    set(_CXX_FLAGS "-O3")
    # set(_CXX_FLAGS  "${_CXX_FLAGS} -march=armv8.4-a")
else ()
    message(STATUS "No suitable compiler flag found for vectorization. We are setting -O3")
    set(_CXX_FLAGS "-O3")
    endif()

find_package(Boost REQUIRED COMPONENTS program_options)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(hw main.cpp)
    target_compile_options(hw
      PRIVATE
      ${_CXX_FLAGS}
    )
    target_link_libraries(hw
      PRIVATE
      ${Boost_LIBRARIES}
    )
    add_executable(bench bench.cpp)
    target_compile_options(bench
       PRIVATE
       ${_CXX_FLAGS}
    )
else ()
    message(FATAL_ERROR "Boost not found")
endif()
