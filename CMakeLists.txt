cmake_minimum_required(VERSION 3.1)
project(metalog)

set(CMAKE_CXX_FLAGS "-O0 -g -Wall -Wextra")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
	include
)

add_executable(${PROJECT_NAME} src/test.cpp)