#include <filesystem>
#include <iostream>
#include <print>
#include <stdexcept>
#include <string>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common/compat.h"
#include "common/errutils.h"
#include "common/glutils.h"
#include "common/shader.h"
#include "common/texture.h"

namespace fs = std::filesystem;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const float ZNEAR = 0.01f;
const float ZFAR = 100.f;
const float FOV = glm::radians(45.f);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    if (width && height)
        glViewport(0, 0, width, height);
}

int LGL_TMAIN(int argc, LGL_TCHAR* argv[]) {
    try {
        fs::path exe_path = fs::canonical(argc ? argv[0] : fs::path());
        fs::path root = exe_path.parent_path();

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

        GLuint shader = load_shader(root / "texture_demo/resources/shaders/shader.vs",
                                    root / "texture_demo/resources/shaders/shader.fs");
        GLuint texture = load_texture(root / "resources/textures/checkerboard.png");

        float vertices[][8] = {
            // Position       | Color        | Tex coords
            {-0.5f, -0.5f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f},
            {+0.5f, -0.5f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f},
            {+0.5f, +0.5f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f},
            {-0.5f, +0.5f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f},
        };
        unsigned int indices[] = {0, 1, 2, 2, 3, 0};

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
                              reinterpret_cast<void*>(0 * sizeof(float)));
        // Color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]),
                              reinterpret_cast<void*>(3 * sizeof(float)));
        // Tex coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]),
                              reinterpret_cast<void*>(6 * sizeof(float)));
        glBindVertexArray(0);

        glUseProgram(shader);

        while (!glfwWindowShouldClose(window)) {
            glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            auto [_x, _y, width, height] = util::gl_get<float, 4>(GL_VIEWPORT);
            float aspect = width / height;
            glm::mat4 projection = glm::perspective(FOV, aspect, ZNEAR, ZFAR);
            // glm::mat4 projection = glm::ortho(-aspect, aspect, -1.f, 1.f, ZNEAR, ZFAR);
            glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE,
                               glm::value_ptr(projection));

            glm::mat4 modelview{1};
            modelview = glm::translate(modelview, glm::vec3(0, 0, -2));
            float angle = float(glfwGetTime()) * glm::pi<float>() / 4.f;
            modelview = glm::rotate(modelview, angle, glm::vec3(0, 1, 0));
            glUniformMatrix4fv(glGetUniformLocation(shader, "modelview"), 1, GL_FALSE,
                               glm::value_ptr(modelview));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            glUniform1i(glGetUniformLocation(shader, "tex"), 0);

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
