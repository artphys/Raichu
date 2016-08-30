cmake_minimum_required(VERSION 3.6)
project(Raichu)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

include_directories(C:/Users/ironstudent/TX )

set(SOURCE_FILES main.cpp Field.cpp Objects.cpp Point.cpp)
add_executable(Raichu ${SOURCE_FILES})