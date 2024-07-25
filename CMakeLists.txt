cmake_minimum_required(VERSION 3.29)
project(asciichis)

set(CMAKE_CXX_STANDARD 17)

add_executable(asciichis main.cpp asciichis.cpp
        viewmanager.cpp
        viewmanager.h
        Drawables/drawable.cpp
        Drawables/drawable.h
        Drawables/line.cpp
        Drawables/line.h)
