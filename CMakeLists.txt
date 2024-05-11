cmake_minimum_required(VERSION 3.22)
project(Lab1Crypto)

set(CMAKE_CXX_STANDARD 23)

add_executable(Lab1Crypto lab1.cpp lab2.cpp
        additional_func.h
        additional_func.cpp
        main.cpp
        lab1.h
        lab2.h)