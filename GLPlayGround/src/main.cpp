#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <windows.h>
#include "Const.h"
#include "Shader.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

float vertices[] = {
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // top right
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // botom right
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
    -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f}; // top left

unsigned int indices[] = {
    0, 1, 2,
};


const unsigned int WIN_WIDTH = 1024, WIN_WIN_HEIGHT = 768;
ZG::Shader* shader;

void framebuffer_resize_callback(GLFWwindow *window, int WIN_WIDTH, int WIN_WIN_HEIGHT) {
    glViewport(0, 0, WIN_WIDTH, WIN_WIN_HEIGHT);
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

std::string getexepath() {
    char result[MAX_PATH];
    return std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
}

void Render(unsigned int VAO) {

    shader->use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

int main(int argc, char **argv) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIN_WIDTH, WIN_WIN_HEIGHT, "Test Window", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create window " << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to load glad function" << endl;
        return -1;
    }

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);  // 说明这是location 0
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);  // 说明这是location 0

   
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 这是unbind vertex array
    glBindVertexArray(0);

    shader = new ZG::Shader("vertex.glsl", "fragment.glsl");

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window)) {
        process_input(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // swap buffer and poll IO events
        Render(VAO);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
