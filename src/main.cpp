#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Classes/Shader.h"
#include "Classes/Camera.h"
#include "Classes/Model.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// Configurações
const unsigned int LARGURA_TELA = 1200;
const unsigned int ALTURA_TELA = 800;

// Camera
Camera camera(glm::vec3(3750.0f, 1500.0f, -1000.0f), glm::vec3(0, 1.0f, -0.1f));
float ultimoX = LARGURA_TELA / 2.0f;
float ultimoY = ALTURA_TELA / 2.0f;
bool firstMouse = true;

// Tempo
float intervaloEntreFrames = 0.0f;
float tempoDoUltimoFrame = 0.0f;
float tempo = 0.0f;

int main()
{
    // Configuração básica
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    //Criação da tela
    GLFWwindow* window = glfwCreateWindow(LARGURA_TELA, ALTURA_TELA, "Sistema Solar", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //Set dos callbacks
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //Modo do mouse, desativa o posicionamento do cursor para implmenetar a câmera
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Modifica o stb_image.h para carregar texturas no eixo y, configuração padrã.
    stbi_set_flip_vertically_on_load(true);

    // Diz ao openGl para tratar da profundidade
    glEnable(GL_DEPTH_TEST);


    //Shaders
    Shader planetas_shader("resources/Shaders/model_loading.vert", "resources/Shaders/model_loading.frag");
    Shader cor_shader("resources/Shaders/model_loading.vert", "resources/Shaders/color.frag");

    // load models
    // -----------
    Model Sun("resources/Models/Sun/Sun.obj");
    Model Mercury("resources/Models/Mercury/Mercury.obj");
    Model Venus("resources/Models/Venus/Venus.obj");
    Model Earth("resources/Models/Earth/Earth.obj");
    Model Moon("resources/Models/Moon/Moon.obj");
    Model Mars("resources/Models/Mars/Mars.obj");
    Model Jupiter("resources/Models/Jupiter/Jupiter.obj");
    Model Saturn("resources/Models/Saturn/Saturn.obj");
    Model Uranus("resources/Models/Uranus/Uranus.obj");
    Model Neptune("resources/Models/Neptune/Neptune.obj");

    //Models utilitários
    Model Background("resources/Models/Background/Background.obj");
    Model Orbita("resources/Models/Line/Line.obj");
    Model Orbita2("resources/Models/Line2/Line2.obj");
    Model Orbita3("resources/Models/Line3/Line3.obj");


    // Loop principal do sistema
    while (!glfwWindowShouldClose(window))
    {
        // Cálculo do tempo e dos frames, o tempo é calculado com base no tempo em que o último frame foi modificado
        float frameAtual = static_cast<float>(glfwGetTime());
        intervaloEntreFrames = frameAtual - tempoDoUltimoFrame;
        tempoDoUltimoFrame = frameAtual;
        tempo += intervaloEntreFrames;

        //Input do usuário
        processInput(window);


        // ---------------------------- RENDERIZAÇÃO ---------------------------- //

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        planetas_shader.use();

        //Matrizes de visualização do mundo, define o campo de visão com base no zoom da câmera
        glm::mat4 projecao = glm::perspective(glm::radians(camera.Zoom), (float) LARGURA_TELA / (float)ALTURA_TELA, 0.1f, 25000.0f);
        glm::mat4 visualizacao = camera.GetViewMatrix();
        planetas_shader.setMat4("projection", projecao);
        planetas_shader.setMat4("view", visualizacao);

        glm::mat4 background = glm::mat4(1.0f);
        background = glm::translate(background, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        background = glm::scale(background, glm::vec3(4000, 4000, 4000));
        planetas_shader.setMat4("model", background);
        Background.Draw(planetas_shader);

        // render the loaded model
        glm::mat4 sun = glm::mat4(1.0f);
        sun = glm::translate(sun, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        sun = glm::scale(sun, glm::vec3(50, 50, 50));
        planetas_shader.setMat4("model", sun);
        Sun.Draw(planetas_shader);

        glm::mat4 mercury = glm::mat4(1.0f);
        float mercuryScale = 10;
        mercury = glm::translate(mercury, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        mercury = glm::scale(mercury, glm::vec3(mercuryScale, mercuryScale, mercuryScale));
        mercury = glm::rotate(mercury, tempo * 4, glm::vec3(0.0f, 1.0f, 0.0f));
        mercury = glm::translate(mercury, glm::vec3(0.0f, 0.0f, 17.5f)); // translate it down so it's at the center of the scene
        planetas_shader.setMat4("model", mercury);
        Mercury.Draw(planetas_shader);

        glm::mat4 venus = glm::mat4(1.0f);
        venus = glm::translate(venus, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        venus = glm::scale(venus, glm::vec3(15, 15, 15));
        venus = glm::rotate(venus, tempo * 1.5f, glm::vec3(0.0f, 1.0f, 0.0f));// it's a bit too big for our scene, so scale it down
        venus = glm::translate(venus, glm::vec3(0.0f, 0.0f, 22)); // translate it down so it's at the center of the scene
        planetas_shader.setMat4("model", venus);
        Venus.Draw(planetas_shader);

        glm::mat4 earth = glm::mat4(1.0f);
        earth = glm::translate(earth, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        earth = glm::scale(earth, glm::vec3(17, 17, 17));
        earth = glm::rotate(earth, tempo, glm::vec3(0.0f, 1.0f, 0.0f));// it's a bit too big for our scene, so scale it down
        earth = glm::translate(earth, glm::vec3(0.0f, 0.0f, 26)); // translate it down so it's at the center of the scene
        planetas_shader.setMat4("model", earth);
        Earth.Draw(planetas_shader);
        //lua
        earth = glm::scale(earth, glm::vec3(0.5, 0.5, 0.5));
        earth = glm::rotate(earth, tempo, glm::vec3(0.0f, 1.0f, 0.0f));// it's a bit too big for our scene, so scale it down
        earth = glm::translate(earth, glm::vec3(-3, 0, 8));
        planetas_shader.setMat4("model", earth);
        Moon.Draw(planetas_shader);

        glm::mat4 mars = glm::mat4(1.0f);
        mars = glm::translate(mars, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        mars = glm::scale(mars, glm::vec3(13, 13, 13));
        mars = glm::rotate(mars, tempo / 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));// it's a bit too big for our scene, so scale it down
        mars = glm::translate(mars, glm::vec3(0.0f, 0.0f, 50)); // translate it down so it's at the center of the scene
        planetas_shader.setMat4("model", mars);
        Mars.Draw(planetas_shader);

        glm::mat4 jupiter = glm::mat4(1.0f);
        jupiter = glm::translate(jupiter, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        jupiter = glm::scale(jupiter, glm::vec3(45,45, 45));
        jupiter = glm::rotate(jupiter, tempo / 4, glm::vec3(0.0f, 1.0f, 0.0f));// it's a bit too big for our scene, so scale it down
        jupiter = glm::translate(jupiter, glm::vec3(0.0f, 0.0f, 30)); // translate it down so it's at the center of the scene
        planetas_shader.setMat4("model", jupiter);
        Jupiter.Draw(planetas_shader);

        glm::mat4 saturn = glm::mat4(1.0f);
        saturn = glm::translate(saturn, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        saturn = glm::scale(saturn, glm::vec3(42, 42, 42));
        saturn = glm::rotate(saturn, tempo / 6, glm::vec3(0.0f, 1.0f, 0.0f));// it's a bit too big for our scene, so scale it down
        saturn = glm::translate(saturn, glm::vec3(0.0f, 0.0f, 60)); // translate it down so it's at the center of the scene
        planetas_shader.setMat4("model", saturn);
        Saturn.Draw(planetas_shader);
        saturn = glm::scale(saturn, glm::vec3(4, 4, 4));
        saturn = glm::rotate(saturn, (float) glfwGetTime()-60, glm::vec3(0.0f, 1.0f, 0.0f));// it's a bit too big for our scene, so scale it down
        saturn = glm::translate(saturn, glm::vec3(0.0f, 0.0f, 0)); // translate it down so it's at the center of the scene
        planetas_shader.setMat4("model", saturn);
        Orbita3.Draw(planetas_shader);

        glm::mat4 uranus = glm::mat4(1.0f);
        uranus = glm::translate(uranus, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        uranus = glm::scale(uranus, glm::vec3(30, 30, 30));
        uranus = glm::rotate(uranus, tempo / 8, glm::vec3(0.0f, 1.0f, 0.0f));// it's a bit too big for our scene, so scale it down
        uranus = glm::translate(uranus, glm::vec3(0.0f, 0.0f, 120)); // translate it down so it's at the center of the scene
        planetas_shader.setMat4("model", uranus);
        Uranus.Draw(planetas_shader);

        glm::mat4 neptune = glm::mat4(1.0f);
        neptune = glm::translate(neptune, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        neptune = glm::scale(neptune, glm::vec3(29, 29, 29));
        neptune = glm::rotate(neptune, tempo / 10, glm::vec3(0.0f, 1.0f, 0.0f));// it's a bit too big for our scene, so scale it down
        neptune = glm::translate(neptune, glm::vec3(0.0f, 0.0f, 180)); // translate it down so it's at the center of the scene
        planetas_shader.setMat4("model", neptune);
        Neptune.Draw(planetas_shader);
        neptune = glm::scale(neptune, glm::vec3(4, 4, 4));
        neptune = glm::rotate(neptune, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));// it's a bit too big for our scene, so scale it down
        neptune = glm::translate(neptune, glm::vec3(0.0f, 0.0f, 0)); // translate it down so it's at the center of the scene
        planetas_shader.setMat4("model", neptune);
        Orbita3.Draw(planetas_shader);

        cor_shader.use();

        cor_shader.setMat4("projection", projecao);
        cor_shader.setMat4("view", visualizacao);


        glm::mat4 orbitaMercurio = glm::mat4(1.0f);
        float orbitaMercurioScale = 180;
        orbitaMercurio = glm::translate(orbitaMercurio, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        orbitaMercurio = glm::scale(orbitaMercurio, glm::vec3(orbitaMercurioScale, orbitaMercurioScale, orbitaMercurioScale));
        cor_shader.setMat4("model", orbitaMercurio);
        Orbita.Draw(cor_shader);

        glm::mat4 orbitaVenus = glm::mat4(1.0f);
        float orbitaVenusScale = 350;
        orbitaVenus = glm::translate(orbitaVenus, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        orbitaVenus = glm::scale(orbitaVenus, glm::vec3(orbitaVenusScale, orbitaVenusScale, orbitaVenusScale));
        cor_shader.setMat4("model", orbitaVenus);
        Orbita.Draw(cor_shader);

        glm::mat4 orbitaTerra = glm::mat4(1.0f);
        float orbitaTerraScale = 450;
        orbitaTerra = glm::translate(orbitaTerra, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        orbitaTerra = glm::scale(orbitaTerra, glm::vec3(orbitaTerraScale, orbitaTerraScale, orbitaTerraScale));
        cor_shader.setMat4("model", orbitaTerra);
        Orbita.Draw(cor_shader);

        glm::mat4 orbitaMarte = glm::mat4(1.0f);
        float orbitaMarteScale = 655;
        orbitaMarte = glm::translate(orbitaMarte, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        orbitaMarte = glm::scale(orbitaMarte, glm::vec3(orbitaMarteScale, orbitaMarteScale, orbitaMarteScale));
        cor_shader.setMat4("model", orbitaMarte);
        Orbita.Draw(cor_shader);

        glm::mat4 orbitaJupiter = glm::mat4(1.0f);
        float orbitaJupiterScale = 1350;
        orbitaJupiter = glm::translate(orbitaJupiter, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        orbitaJupiter = glm::scale(orbitaJupiter, glm::vec3(orbitaJupiterScale, orbitaJupiterScale, orbitaJupiterScale));
        cor_shader.setMat4("model", orbitaJupiter);
        Orbita2.Draw(cor_shader);

        glm::mat4 orbitaSaturno = glm::mat4(1.0f);
        float orbitaSaturnoScale = 2550;
        orbitaSaturno = glm::translate(orbitaSaturno, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        orbitaSaturno = glm::scale(orbitaSaturno, glm::vec3(orbitaSaturnoScale, orbitaSaturnoScale, orbitaSaturnoScale));
        cor_shader.setMat4("model", orbitaSaturno);
        Orbita2.Draw(cor_shader);

        glm::mat4 orbitaUrano = glm::mat4(1.0f);
        float orbitaUranoScale = 3650;
        orbitaUrano = glm::translate(orbitaUrano, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        orbitaUrano = glm::scale(orbitaUrano, glm::vec3(orbitaUranoScale, orbitaUranoScale, orbitaUranoScale));
        cor_shader.setMat4("model", orbitaUrano);
        Orbita2.Draw(cor_shader);

        glm::mat4 orbitaNetuno = glm::mat4(1.0f);
        float orbitaNetunoScale = 5300;
        orbitaNetuno = glm::translate(orbitaNetuno, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        orbitaNetuno = glm::scale(orbitaNetuno, glm::vec3(orbitaNetunoScale, orbitaNetunoScale, orbitaNetunoScale));
        cor_shader.setMat4("model", orbitaNetuno);
        Orbita2.Draw(cor_shader);


        //Buffer de cores e eventos de entrada
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

//Processa o input
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, intervaloEntreFrames);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, intervaloEntreFrames);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, intervaloEntreFrames);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, intervaloEntreFrames);
}

//Callback de dimensão da tela
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    //Posição atual do mouse
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        ultimoX = xpos;
        ultimoY = ypos;
        firstMouse = false;
    }

    //Qunatidade que variou
    float xoffset = xpos - ultimoX;
    float yoffset = ultimoY - ypos; // reversed since y-coordinates go from bottom to top

    ultimoX = xpos;
    ultimoY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}