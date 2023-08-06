#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>
#include <shader.h>
#include <camera.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow *window);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

// settings
unsigned int scr_width = 800;
unsigned int scr_height = 600;

enum MOUSE_BUTTONS {
    LEFT_MOUSE_KEY,
    RIGHT_MOUSE_KEY,
    WHEEL_MOUSE_KEY,
};

std::vector<glm::vec3> cubePositions = {};

bool isRightMouseKeyBeingPressed = false;

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(scr_width, scr_height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    ImGui_ImplGlfwGL3_Init(window, true);

    // Setup style
    ImGui::StyleColorsDark();

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // build and compile our shader zprogram
    Shader ourShader("src/shaders/shader.vs", "src/shaders/shader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        // vertices          // colors
        0.5f,   0.5f, 0.0f,  1.0f, 1.0f, 1.0f, // top right
        0.5f,  -0.5f, 0.0f,  1.0f, 1.0f, 1.0f, // bottom right
        -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f, // bottom left
        -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f  // top left 
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
    // colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glBindVertexArray(0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        {
            ImGui::Begin("Performance");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }


        float aspect = (float)scr_width/(float)scr_height;

        for (glm::vec3 pos: cubePositions){

            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -0.1f, 100.0f);
            ourShader.setMat4("projection", projection);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pos);
            model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
            ourShader.setMat4("model", model);

            ourShader.use();
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        }

        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        glClearColor(0.145f, 0.145f, 0.145f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    scr_height = height;
    scr_width = width;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS && button == LEFT_MOUSE_KEY){
        isRightMouseKeyBeingPressed = true;
        // should place a rectangle
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        float coordx = 2 * (float)xpos/scr_width - 1;
        float coordy = - 2 * (float)ypos/scr_height + 1;
        glm::vec3 pos = glm::vec3(coordx, coordy,  0.0f);
        cubePositions.push_back(pos);

        // Print the mouse cursor's position
        std::cout << "Mouse Position: x = " << xpos << ", y = " << ypos << std::endl;
    } else if (action == GLFW_RELEASE && button == LEFT_MOUSE_KEY) {
        isRightMouseKeyBeingPressed = false;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
    if (isRightMouseKeyBeingPressed)
    {
        float coordx = 2 * (float)xpos / scr_width - 1;
        float coordy = -2 * (float)ypos / scr_height + 1;
        glm::vec3 newPos = glm::vec3(coordx, coordy, 0.0f);

        // Always add the last recorded position along with the new position
        if (!cubePositions.empty()) {
            glm::vec3 lastPos = cubePositions.back();
            float distance = glm::length(newPos - lastPos);

            // Only add intermediate positions if the distance is significant
            const float minDistance = 0.007f; // Adjust this threshold as needed
            if (distance > minDistance) {
                int numSteps = static_cast<int>(distance / minDistance);
                for (int i = 0; i < numSteps; ++i) {
                    glm::vec3 interpolatedPos = glm::mix(lastPos, newPos, static_cast<float>(i) / numSteps);
                    cubePositions.push_back(interpolatedPos);
                }
            }
        }

        // Add the new position to the end of the positions array
        cubePositions.push_back(newPos);
    }
}
