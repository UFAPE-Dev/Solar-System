cmake_minimum_required(VERSION 3.22)
project (SolarSystem)

#Adiciona os arquivos CMake que encontram as bibliotecas
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
#Para linux
set(OpenGlLinkers -lglut -lGLU -lX11 -lpthread -lXrandr -lXi -ldl -lassimp)

#Usa o pacote para encontrar bibliotecas
include(FindPkgConfig)
#Define a versão do CMake
set(CMAKE_CXX_STANDARD 14)

#Procura o GLFW
find_package(GLFW REQUIRED)

#Adiciona o GLAD
add_library(glad libraries/GLAD/lib/glad.c libraries/GLAD/lib/glad.c)
target_include_directories(glad PUBLIC libraries/GLAD/lib)





#Nome do executável
add_executable(${PROJECT_NAME}
        main.cpp main.h
        libraries/GLAD/lib/glad.c
        Classes/Mesh.h
        Classes/Shader.h
        Classes/Model.h Classes/Camera.h)



#Diretório de headers
include_directories(${GLFW_INCLUDE_DIR} ${GLAD_INCLUDE_DIR} libraries/STB/lib)
#Link das bibliotecas
target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARY} ${OpenGlLinkers} glad stb)
