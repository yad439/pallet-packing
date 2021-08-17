cmake_minimum_required(VERSION 3.20)
project(pallet-packing-cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/.." CACHE PATH "..." FORCE)
endif ()

add_library(cpp MODULE library.cpp skyline.cpp permutation_encoding.cpp permutation_encoding.hpp)
add_executable(workshop main.cpp NFD.cpp o-tree.cpp side_fucnctions.cpp simulated_annealing.cpp)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
	target_compile_options(cpp PRIVATE -Wall -Wextra -Wpedantic)
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
	target_compile_options(cpp PRIVATE /W4 /analyze)
endif ()

install(TARGETS cpp DESTINATION "lib")
install(TARGETS workshop)