cmake_minimum_required(VERSION 3.0.0)
project(solar_system VERSION 0.1.0)

include(CTest)
enable_testing()

set (GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set (GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set (GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ldl -lGLESv2 -lassimp")

set(BUILD_STATIC_LIB ON)

add_subdirectory(ext/glfw)
add_subdirectory(ext/glad)
add_subdirectory(ext/glm)

include_directories(ext/STB/lib)
add_subdirectory(src)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)