#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common/errutils.h"
#include "common/glutils.h"
#include "common/shader.h"

namespace fs = std::filesystem;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const float ZNEAR = 0.01f;
const float ZFAR = 100.f;
const float FOV = glm::radians(45.f);

const char VS_SOURCE[] = R"(
    #version 330

    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 color;
    out vec3 v_color;

    uniform mat4 modelview;
    uniform mat4 projection;

    void main() {
        gl_Position = projection * (modelview * vec4(position, 1.));
        v_color = color;
    }
)";

const char FS_SOURCE[] = R"(
    #version 330

    in vec3 v_color;
    out vec4 color;

    void main() {
        color = vec4(v_color, 1.);
    }
)";

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    if (width && height)
        glViewport(0, 0, width, height);
}

#ifdef _WIN32
// Support unicode args on Windows
int wmain(int argc, wchar_t* argv[]) {
#else
int main(int argc, char* argv[]) {
#endif
    try {
        err::check_glfw(glfwInit(), "failed to init GLFW: {}");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL",
                                              nullptr, nullptr);
        err::check_glfw(window, "failed to create GLFW window: {}");
        glfwMakeContextCurrent(window);
        err::check_glfw(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress),
                        "failed to load GL loader: {}");
        glfwSetKeyCallback(window, key_callback);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        GLuint shader = build_shader(VS_SOURCE, FS_SOURCE);

        float vertices[][6] = {
            // Position       | Color
            {-0.6f, -0.5f, 0.f, 1.f, 0.f, 0.f},
            {+0.6f, -0.5f, 0.f, 0.f, 1.f, 0.f},
            {+0.0f, +0.5f, 0.f, 0.f, 0.f, 1.f},
        };
        unsigned int indices[] = {0, 1, 2};

        GLuint vao, vbo, ebo;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]),
                              reinterpret_cast<void*>(0));
        // Color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]),
                              reinterpret_cast<void*>(3 * sizeof(float)));

        glBindVertexArray(0);

        glUseProgram(shader);

        while (!glfwWindowShouldClose(window)) {
            glClearColor(0.8f, 0.8f, .8f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            auto [_x, _y, width, height] = util::gl_get<float, 4>(GL_VIEWPORT);
            float aspect = width / height;
            glm::mat4 projection = glm::perspective(FOV, aspect, ZNEAR, ZFAR);
            // glm::mat4 projection = glm::ortho(-aspect, aspect, -1.f, 1.f, ZNEAR, ZFAR);
            glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE,
                               glm::value_ptr(projection));

            glm::mat4 modelview{1};
            modelview = glm::translate(modelview, glm::vec3(0, 0, -2));
            float angle = float(glfwGetTime()) * glm::pi<float>() / 2.f;
            modelview = glm::rotate(modelview, angle, glm::vec3(0, 1, 0));
            glUniformMatrix4fv(glGetUniformLocation(shader, "modelview"), 1, GL_FALSE,
                               glm::value_ptr(modelview));

            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, GLsizei(std::size(indices)), GL_UNSIGNED_INT, 0);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        glfwTerminate();
        return 1;
    }
    glfwTerminate();
    return 0;
}
