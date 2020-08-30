cmake_minimum_required(VERSION 3.10)
project(SteinerTree)

set(CMAKE_CXX_STANDARD 17)

add_executable(SteinerTree src/main.cpp src/type_define.h src/SolutionKlein.cpp src/SolutionKlein.h src/SolutionOpt.cpp src/SolutionOpt.h src/GraphGenerator.h src/GraphGenerator.cpp)