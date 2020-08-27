cmake_minimum_required(VERSION 3.17)
project(SteinerTree)

set(CMAKE_CXX_STANDARD 17)

add_executable(SteinerTree src/main.cpp src/type_define.h src/SolutionKlein.cpp src/SolutionKlein.h src/SolutionOpt.cpp src/SolutionOpt.h)