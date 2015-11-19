cmake_minimum_required( VERSION 2.6 )
project(gameoflifecl)

#Set variable
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")
set(CMAKE_CXX_FLAGS "-O3 -DDEBUG_MODE -Wall")
set(CMAKE_SHARED_LINKER_FLAGS)

#Check for OpenCL
find_package(OpenCL 1.2 EXACT)
find_package(CUDA REQUIRED)
include_directories( ${OPENCL_INCLUDE_DIRS} )
include_directories( ${CMAKE_SOURCE_DIR}/include/ )

message(STATUS "OpenCL found: ${OPENCL_FOUND}")
message(STATUS "OpenCL includes: ${OPENCL_INCLUDE_DIRS}")
message(STATUS "OpenCL CXX includes: ${OPENCL_HAS_CPP_BINDINGS}")
message(STATUS "OpenCL libraries: ${OPENCL_LIBRARIES}")

#Add source files for binary
file(GLOB SRC "${CMAKE_SOURCE_DIR}/src/*.cpp")

if(OPENCL_HAS_CPP_BINDINGS)
    message("OpenCL has C++ bindings!")
# 	message(STATUS "OpenCL found: ${OPENCL_FOUND}")

	#Build
	add_executable( gameoflifecl main.cpp ${SRC} )
	target_link_libraries( gameoflifecl OpenCL )
else(OPENCL_HAS_CPP_BINDINGS)
    message("No C++ bindings found for OpenCL! aborting...")
endif(OPENCL_HAS_CPP_BINDINGS)