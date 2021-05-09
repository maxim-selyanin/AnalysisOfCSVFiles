cmake_minimum_required(VERSION 3.16)
project(csvSort)

set(CMAKE_CXX_STANDARD 17)

add_executable(csvSort main.cpp stats.h icsv.cpp icsv.h common.h table.h string_iteration.cpp string_iteration.h csv_format.h my_sort.h corr.h)