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

// settings
const unsigned int SCR_WIDTH = 900;
const unsigned int SCR_HEIGHT = 700;
const float SUN_SIZE = 30.0;

// camera
Camera camera(glm::vec3(3750.0f, 1500.0f, -1000.0f), glm::vec3(0, 1.0f, -0.1f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("resources/Shaders/model_loading.vert", "resources/Shaders/model_loading.frag");
    Shader colorShader("resources/Shaders/model_loading.vert", "resources/Shaders/color.frag");

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

    Model Background("resources/Models/Background/Background.obj");
    Model Line("resources/Models/Line/Line.obj");
    Model Line2("resources/Models/Line2/Line2.obj");


    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, SUN_SIZE + 25000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        glm::mat4 background = glm::mat4(1.0f);
        background = glm::translate(background, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        background = glm::scale(background, glm::vec3(3500, 3500, 3500));
        ourShader.setMat4("model", background);
        Background.Draw(ourShader);

        // render the loaded model
        glm::mat4 sun = glm::mat4(1.0f);
        sun = glm::translate(sun, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        sun = glm::scale(sun, glm::vec3(50, 50, 50));
        ourShader.setMat4("model", sun);
        Sun.Draw(ourShader);

        glm::mat4 mercury = glm::mat4(1.0f);
        float mercuryScale = 10;
        mercury = glm::translate(mercury, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        mercury = glm::scale(mercury, glm::vec3(mercuryScale, mercuryScale, mercuryScale));
        mercury = glm::rotate(mercury, (float) glfwGetTime() -10, glm::vec3(0.0f, 1.0f, 0.0f));
        mercury = glm::translate(mercury, glm::vec3(0.0f, 0.0f, 17.5f)); // translate it down so it's at the center of the scene
        ourShader.setMat4("model", mercury);
        Mercury.Draw(ourShader);

        glm::mat4 venus = glm::mat4(1.0f);
        venus = glm::translate(venus, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        venus = glm::scale(venus, glm::vec3(15, 15, 15));
        venus = glm::rotate(venus, (float) glfwGetTime() -20, glm::vec3(0.0f, 1.0f, 0.0f));// it's a bit too big for our scene, so scale it down
        venus = glm::translate(venus, glm::vec3(0.0f, 0.0f, 22)); // translate it down so it's at the center of the scene
        ourShader.setMat4("model", venus);
        Venus.Draw(ourShader);

        glm::mat4 earth = glm::mat4(1.0f);
        earth = glm::translate(earth, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        earth = glm::scale(earth, glm::vec3(17, 17, 17));
        earth = glm::rotate(earth, (float) glfwGetTime() -30, glm::vec3(0.0f, 1.0f, 0.0f));// it's a bit too big for our scene, so scale it down
        earth = glm::translate(earth, glm::vec3(0.0f, 0.0f, 26)); // translate it down so it's at the center of the scene
        ourShader.setMat4("model", earth);
        Earth.Draw(ourShader);
        //lua
        earth = glm::scale(earth, glm::vec3(0.5, 0.5, 0.5));
        earth = glm::rotate(earth, (float) glfwGetTime() -30, glm::vec3(0.0f, 1.0f, 0.0f));// it's a bit too big for our scene, so scale it down
        earth = glm::translate(earth, glm::vec3(-3, 0, 8));
        ourShader.setMat4("model", earth);
        Moon.Draw(ourShader);

        glm::mat4 moon = glm::mat4(1.0f);
        /*moon = glm::translate(moon, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        moon = glm::scale(moon, glm::vec3(10, 10, 10));

        moon = glm::translate(moon, glm::vec3(43, 0, 5)); // translate it down so it's at the center of the scene
        ourShader.setMat4("model", moon);*/


        glm::mat4 mars = glm::mat4(1.0f);
        mars = glm::translate(mars, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        mars = glm::scale(mars, glm::vec3(13, 13, 13));
        mars = glm::rotate(mars, (float) glfwGetTime()-40, glm::vec3(0.0f, 1.0f, 0.0f));// it's a bit too big for our scene, so scale it down
        mars = glm::translate(mars, glm::vec3(0.0f, 0.0f, 50)); // translate it down so it's at the center of the scene
        ourShader.setMat4("model", mars);
        Mars.Draw(ourShader);

        glm::mat4 jupiter = glm::mat4(1.0f);
        jupiter = glm::translate(jupiter, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        jupiter = glm::scale(jupiter, glm::vec3(45,45, 45));
        jupiter = glm::rotate(jupiter, (float) glfwGetTime()-50, glm::vec3(0.0f, 1.0f, 0.0f));// it's a bit too big for our scene, so scale it down
        jupiter = glm::translate(jupiter, glm::vec3(0.0f, 0.0f, 30)); // translate it down so it's at the center of the scene
        ourShader.setMat4("model", jupiter);
        Jupiter.Draw(ourShader);

        glm::mat4 saturn = glm::mat4(1.0f);
        saturn = glm::translate(saturn, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        saturn = glm::scale(saturn, glm::vec3(42, 42, 42));
        saturn = glm::rotate(saturn, (float) glfwGetTime()-60, glm::vec3(0.0f, 1.0f, 0.0f));// it's a bit too big for our scene, so scale it down
        saturn = glm::translate(saturn, glm::vec3(0.0f, 0.0f, 60)); // translate it down so it's at the center of the scene
        ourShader.setMat4("model", saturn);
        Saturn.Draw(ourShader);

        glm::mat4 uranus = glm::mat4(1.0f);
        uranus = glm::translate(uranus, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        uranus = glm::scale(uranus, glm::vec3(30, 30, 30));
        uranus = glm::rotate(uranus, (float) glfwGetTime()-70, glm::vec3(0.0f, 1.0f, 0.0f));// it's a bit too big for our scene, so scale it down
        uranus = glm::translate(uranus, glm::vec3(0.0f, 0.0f, 120)); // translate it down so it's at the center of the scene
        ourShader.setMat4("model", uranus);
        Uranus.Draw(ourShader);

        glm::mat4 neptune = glm::mat4(1.0f);
        neptune = glm::translate(neptune, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        neptune = glm::scale(neptune, glm::vec3(29, 29, 29));
        neptune = glm::rotate(neptune, (float) glfwGetTime()-80, glm::vec3(0.0f, 1.0f, 0.0f));// it's a bit too big for our scene, so scale it down
        neptune = glm::translate(neptune, glm::vec3(0.0f, 0.0f, 180)); // translate it down so it's at the center of the scene
        ourShader.setMat4("model", neptune);
        Neptune.Draw(ourShader);

        colorShader.use();

        colorShader.setMat4("projection", projection);
        colorShader.setMat4("view", view);


        glm::mat4 orbitaMercurio = glm::mat4(1.0f);
        float orbitaMercurioScale = 180;
        orbitaMercurio = glm::translate(orbitaMercurio, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        orbitaMercurio = glm::scale(orbitaMercurio, glm::vec3(orbitaMercurioScale, orbitaMercurioScale, orbitaMercurioScale));
        colorShader.setMat4("model", orbitaMercurio);
        Line.Draw(colorShader);

        glm::mat4 orbitaVenus = glm::mat4(1.0f);
        float orbitaVenusScale = 350;
        orbitaVenus = glm::translate(orbitaVenus, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        orbitaVenus = glm::scale(orbitaVenus, glm::vec3(orbitaVenusScale, orbitaVenusScale, orbitaVenusScale));
        colorShader.setMat4("model", orbitaVenus);
        Line.Draw(colorShader);

        glm::mat4 orbitaTerra = glm::mat4(1.0f);
        float orbitaTerraScale = 450;
        orbitaTerra = glm::translate(orbitaTerra, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        orbitaTerra = glm::scale(orbitaTerra, glm::vec3(orbitaTerraScale, orbitaTerraScale, orbitaTerraScale));
        colorShader.setMat4("model", orbitaTerra);
        Line.Draw(colorShader);

        glm::mat4 orbitaMarte = glm::mat4(1.0f);
        float orbitaMarteScale = 655;
        orbitaMarte = glm::translate(orbitaMarte, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        orbitaMarte = glm::scale(orbitaMarte, glm::vec3(orbitaMarteScale, orbitaMarteScale, orbitaMarteScale));
        colorShader.setMat4("model", orbitaMarte);
        Line.Draw(colorShader);

        glm::mat4 orbitaJupiter = glm::mat4(1.0f);
        float orbitaJupiterScale = 1350;
        orbitaJupiter = glm::translate(orbitaJupiter, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        orbitaJupiter = glm::scale(orbitaJupiter, glm::vec3(orbitaJupiterScale, orbitaJupiterScale, orbitaJupiterScale));
        colorShader.setMat4("model", orbitaJupiter);
        Line2.Draw(colorShader);

        glm::mat4 orbitaSaturno = glm::mat4(1.0f);
        float orbitaSaturnoScale = 2550;
        orbitaSaturno = glm::translate(orbitaSaturno, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        orbitaSaturno = glm::scale(orbitaSaturno, glm::vec3(orbitaSaturnoScale, orbitaSaturnoScale, orbitaSaturnoScale));
        colorShader.setMat4("model", orbitaSaturno);
        Line2.Draw(colorShader);

        glm::mat4 orbitaUrano = glm::mat4(1.0f);
        float orbitaUranoScale = 3650;
        orbitaUrano = glm::translate(orbitaUrano, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        orbitaUrano = glm::scale(orbitaUrano, glm::vec3(orbitaUranoScale, orbitaUranoScale, orbitaUranoScale));
        colorShader.setMat4("model", orbitaUrano);
        Line2.Draw(colorShader);

        glm::mat4 orbitaNetuno = glm::mat4(1.0f);
        float orbitaNetunoScale = 5300;
        orbitaNetuno = glm::translate(orbitaNetuno, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        orbitaNetuno = glm::scale(orbitaNetuno, glm::vec3(orbitaNetunoScale, orbitaNetunoScale, orbitaNetunoScale));
        colorShader.setMat4("model", orbitaNetuno);
        Line2.Draw(colorShader);


     // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}