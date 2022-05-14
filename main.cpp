#include <iostream>
#include <glad.h>
#include <GLFW/glfw3.h>
#include "main.h"
#include <assimp/scene.h>

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //Configura o GLFW para a verssão 3.3 do opengl
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //Compatibilidade do MACOS


    //Incializa a janela do glfw
    GLFWwindow* window = glfwCreateWindow(800, 600, "SolarSystem", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //configura o cursor da janela
    glViewport(0, 0, 800, 600);
    //Função chamada para quando a janela tem seu tamanho alterado
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    //Loop principal do GLFW
    while(!glfwWindowShouldClose(window))
    {
        //Cuida do input
        processInput(window);

        //####################################

        //Renderização vai aqui

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);




        //####################################
        //Troca a matriz de cor em cada renderização
        glfwSwapBuffers(window);
        //Monitora eventos de entrada
        glfwPollEvents();
    }

    //Fim do programa
    glfwTerminate();
    return 0;
}

//Função chamada para quando a janela tem seu tamanho alterado
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    //Indica que o programa deve fechar com GLFW_KEY_ESCAPE e glfwSetWindowShouldClose
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
