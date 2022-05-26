cmake_minimum_required(VERSION 3.0.0)


add_executable(solar_system main.cpp)
target_link_libraries(solar_system glfw glad stb glm assimp)