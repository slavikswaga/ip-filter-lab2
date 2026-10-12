cmake_minimum_required(VERSION 3.10.0)
project(ip-filter-lab2 VERSION 0.1.0 LANGUAGES C CXX)

add_executable(ip-filter-lab2 ip_filter.cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)