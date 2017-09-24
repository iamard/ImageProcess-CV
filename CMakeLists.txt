cmake_minimum_required(VERSION 3.9.2)
project(image)

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH bin)

#Bring the headers, such as Student.h into the project
include_directories(inc)
include_directories(src/adapt)
include_directories(src/alloc)
include_directories(src/codec)
include_directories(src/image)
include_directories(src/stream)

#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)

LINK_LIBRARIES("C:/Windows/System32/OpenCL.DLL")

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "test/*.cpp")
 
add_executable(image ${SOURCES})