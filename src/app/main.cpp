#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>
#include <shader.h>
#include <camera.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Paint Canvas", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    Shader ourShader("src/shaders/shader.vs", "src/shaders/shader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        // positions          // colors
         1.0f,  1.0f, 0.0f,   0.5f, 0.5f, 0.5f,   // top right
         1.0f, -1.0f, 0.0f,   0.5f, 0.5f, 0.5f,   // bottom right
        -1.0f, -1.0f, 0.0f,   0.5f, 0.5f, 0.5f,  // bottom left
        -1.0f,  1.0f, 0.0f,   0.5f, 0.5f, 0.5f  // top left 
    };

    unsigned int indices[] = {
        0,1,3,
        1,2,3
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    ourShader.use();

    unsigned int imageWidth = 40;
    unsigned int imageHeight = 20;
    const float MAX_ABS_POSX_VIEWPORT = 0.8f;
    const float MAX_ABS_POSY_VIEWPORT = 0.8f;
    const float MAX_X_VIEWPORT = MAX_ABS_POSX_VIEWPORT + MAX_ABS_POSX_VIEWPORT;
    const float MAX_Y_VIEWPORT = MAX_ABS_POSY_VIEWPORT + MAX_ABS_POSY_VIEWPORT;

    float pixelWidth = MAX_X_VIEWPORT / imageHeight;
    if (imageWidth > imageHeight){
        pixelWidth = MAX_X_VIEWPORT / imageWidth;
    }
    float pixelHeight = pixelWidth;

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glClearColor(0.80f, 0.80f, 0.80f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        // activate shader
        ourShader.use();

        float posx = -MAX_ABS_POSX_VIEWPORT + pixelWidth / 2;
        float posy = MAX_ABS_POSY_VIEWPORT + pixelWidth / 2;

        glBindVertexArray(VAO);

        for (unsigned int i = 0; i < imageHeight; ++i) {
            for (unsigned int j = 0; j < imageWidth; ++j) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::scale(model, glm::vec3(pixelWidth/2, pixelWidth/2, 0.0f));
                ourShader.setMat4("model", model);

                glm::mat4 trans = glm::mat4(1.0f);
                trans = glm::translate(trans, glm::vec3(posx, posy, 0.0f));
                ourShader.setMat4("trans", trans);

                ourShader.use();

                // Draw the rectangle
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                posx = posx + pixelWidth;
            }
            posx = -MAX_ABS_POSX_VIEWPORT + pixelWidth / 2;
            posy = posy - pixelHeight;
        }
    

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
