#include <glad.h>
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
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 1000.0f, 1000.0f));
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
    Shader ourShader("Shaders/model_loading.vert", "Shaders/model_loading.frag");

    // load models
    // -----------
    Model Sun("Models/Sun/Sun.obj");
    Model Mercury("Models/Mercury/Mercury.obj");
    Model Venus("Models/Venus/Venus.obj");
    Model Earth("Models/Earth/Earth.obj");
    Model Moon("Models/Moon/Moon.obj");
    Model Mars("Models/Mars/Mars.obj");
    Model Jupiter("Models/Jupiter/Jupiter.obj");
    Model Saturn("Models/Saturn/Saturn.obj");
    Model Uranus("Models/Uranus/Uranus.obj");
    Model Neptune("Models/Neptune/Neptune.obj");


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
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 25000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // render the loaded model
        glm::mat4 sun = glm::mat4(1.0f);
        sun = glm::translate(sun, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        sun = glm::scale(sun, glm::vec3(1.0f, 1.0f, 1.0f));
        ourShader.setMat4("model", sun);
        Sun.Draw(ourShader);

        glm::mat4 mercury = glm::mat4(1.0f);
        mercury = glm::translate(mercury, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        mercury = glm::scale(mercury, glm::vec3(1.0f, 1.0f, 1.0f));
        mercury = glm::rotate(mercury, (float) glfwGetTime() -10, glm::vec3(0.0f, 1.0f, 1.0f));
        mercury = glm::translate(mercury, glm::vec3(57.9f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        ourShader.setMat4("model", mercury);
        Mercury.Draw(ourShader);

        glm::mat4 venus = glm::mat4(1.0f);
        venus = glm::translate(venus, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        venus = glm::scale(venus, glm::vec3(1.0f, 1.0f, 1.0f));
        venus = glm::rotate(venus, (float) glfwGetTime() -20, glm::vec3(0.0f, 1.0f, 1.0f));// it's a bit too big for our scene, so scale it down
        venus = glm::translate(venus, glm::vec3(108.2f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        ourShader.setMat4("model", venus);
        Venus.Draw(ourShader);

        float earthDistante = 149.6f;
        glm::mat4 earth = glm::mat4(1.0f);
        earth = glm::translate(earth, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        earth = glm::scale(earth, glm::vec3(1.0f, 1.0f, 1.0f));
        earth = glm::rotate(earth, (float) glfwGetTime(), glm::vec3(0.0f, 1.0f, 1.0f));// it's a bit too big for our scene, so scale it down
        earth = glm::translate(earth, glm::vec3(earthDistante, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        ourShader.setMat4("model", earth);
        Earth.Draw(ourShader);

        glm::mat4 moon = glm::mat4(1.0f);
        moon = glm::translate(moon, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        moon = glm::scale(moon, glm::vec3(1.0f, 1.0f, 1.0f));
        moon = glm::rotate(moon, (float) glfwGetTime() -30, glm::vec3(0.0f, 1.0f, 1.0f));// it's a bit too big for our scene, so scale it down
        moon = glm::translate(moon, glm::vec3(earthDistante + 0.384, earthDistante + 0.384, 0.0f)); // translate it down so it's at the center of the scene
        ourShader.setMat4("model", moon);
        Moon.Draw(ourShader);

        glm::mat4 mars = glm::mat4(1.0f);
        mars = glm::translate(mars, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        mars = glm::scale(mars, glm::vec3(1.0f, 1.0f, 1.0f));
        mars = glm::rotate(mars, (float) glfwGetTime() -40, glm::vec3(0.0f, 1.0f, 1.0f));// it's a bit too big for our scene, so scale it down
        mars = glm::translate(mars, glm::vec3(228.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        ourShader.setMat4("model", mars);
        Mars.Draw(ourShader);

        glm::mat4 jupiter = glm::mat4(1.0f);
        jupiter = glm::translate(jupiter, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        jupiter = glm::scale(jupiter, glm::vec3(1.0f, 1.0f, 1.0f));
        jupiter = glm::rotate(jupiter, (float) glfwGetTime() -50, glm::vec3(0.0f, 1.0f, 1.0f));// it's a bit too big for our scene, so scale it down
        jupiter = glm::translate(jupiter, glm::vec3(778.5f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        ourShader.setMat4("model", jupiter);
        Jupiter.Draw(ourShader);

        glm::mat4 saturn = glm::mat4(1.0f);
        saturn = glm::translate(saturn, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        saturn = glm::scale(saturn, glm::vec3(1.0f, 1.0f, 1.0f));
        saturn = glm::rotate(saturn, (float) glfwGetTime() -60, glm::vec3(0.0f, 1.0f, 1.0f));// it's a bit too big for our scene, so scale it down
        saturn = glm::translate(saturn, glm::vec3(1432.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        ourShader.setMat4("model", saturn);
        Saturn.Draw(ourShader);

        glm::mat4 uranus = glm::mat4(1.0f);
        uranus = glm::translate(uranus, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        uranus = glm::scale(uranus, glm::vec3(1.0f, 1.0f, 1.0f));
        uranus = glm::rotate(uranus, (float) glfwGetTime() -70, glm::vec3(0.0f, 1.0f, 1.0f));// it's a bit too big for our scene, so scale it down
        uranus = glm::translate(uranus, glm::vec3(2867.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        ourShader.setMat4("model", uranus);
        Uranus.Draw(ourShader);

        glm::mat4 neptune = glm::mat4(1.0f);
        neptune = glm::translate(neptune, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        neptune = glm::scale(neptune, glm::vec3(1.0f, 1.0f, 1.0f));
        neptune = glm::rotate(neptune, (float) glfwGetTime() -80, glm::vec3(0.0f, 1.0f, 1.0f));// it's a bit too big for our scene, so scale it down
        neptune = glm::translate(neptune, glm::vec3(4515.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        ourShader.setMat4("model", neptune);
        Neptune.Draw(ourShader);

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