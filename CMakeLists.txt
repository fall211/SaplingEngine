cmake_minimum_required(VERSION 3.10)

project(SaplingEngine)

set(CMAKE_CXX_STANDARD 17)

add_executable(Sapling SaplingEngine/main.cpp)

target_include_directories(SaplingEngine Public
	${CMAKE_SOURCE_DIR}/SaplingEngine
)