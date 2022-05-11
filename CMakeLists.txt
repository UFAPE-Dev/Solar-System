cmake_minimum_required(VERSION 3.21)
project(SolarSystem)

set(CMAKE_CXX_STANDARD 14)
set(OpenGlLinkers -lglut -lGLU -lGL)

add_executable(SolarSystem main.cpp Classes/Model.h)
target_link_libraries(SolarSystem ${OpenGlLinkers})
