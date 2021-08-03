cmake_minimum_required(VERSION 3.20)
project(pallet-packing-cpp)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/.." CACHE PATH "..." FORCE)
endif ()
set(CMAKE_INSTALL_BINDIR lib)

add_library(cpp MODULE library.cpp)

install(TARGETS cpp DESTINATION "lib")