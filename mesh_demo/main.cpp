#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common/errutils.h"
#include "common/mesh.h"
#include "common/shader.h"
#include "common/texture.h"
#include "common/utils.h"

namespace fs = std::filesystem;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const float ZNEAR = 0.01f;
const float ZFAR = 100.f;
const float FOV = glm::radians(45.f);

const glm::vec4 BG_COLOR = {0.8f, 0.8f, .8f, 1.0f};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    if (width && height)
        glViewport(0, 0, width, height);
}

Mesh create_sphere(int nlat, int nlon) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    for (int i = 0; i <= nlat; i++) {
        for (int j = 0; j <= nlon; j++) {
            float theta = float(j % nlon) / nlon * glm::two_pi<float>();
            float phi = float(i) / nlat * glm::pi<float>();
            float x = glm::cos(theta) * glm::sin(phi);
            float y = -glm::cos(phi);
            float z = -glm::sin(theta) * glm::sin(phi);
            float u = float(j) / nlon;
            float v = float(i) / nlat;
            vertices.push_back(Vertex{
                .position = {x, y, z}, .normal = {x, y, z}, .tex_coords = {u, v}});
            if (i < nlat && j < nlon) {
                indices.push_back((i + 0) * (nlon + 1) + (j + 0));
                indices.push_back((i + 0) * (nlon + 1) + (j + 1));
                indices.push_back((i + 1) * (nlon + 1) + (j + 1));
                indices.push_back((i + 1) * (nlon + 1) + (j + 1));
                indices.push_back((i + 1) * (nlon + 1) + (j + 0));
                indices.push_back((i + 0) * (nlon + 1) + (j + 0));
            }
        }
    }
    return Mesh(vertices, indices);
}

#ifdef _WIN32
// Support unicode args on Windows
int wmain(int argc, wchar_t* argv[]) {
#else
int main(int argc, char* argv[]) {
#endif
    try {
        fs::path exe_path = fs::weakly_canonical(argc ? argv[0] : fs::path());
        fs::path resource_dir = exe_path.parent_path() / "resources";

        err::check_glfw(glfwInit(), "failed to init GLFW: {}");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL",
                                              nullptr, nullptr);
        err::check_glfw(window, "failed to create GLFW window: {}");
        glfwMakeContextCurrent(window);
        glfwSetKeyCallback(window, key_callback);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        err::check_glfw(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress),
                        "failed to load GL loader: {}");
        glEnable(GL_FRAMEBUFFER_SRGB);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        Shader shader = Shader::load(resource_dir / "shaders/shader.vs",
                                     resource_dir / "shaders/shader.fs");
        Texture texture(resource_dir / "textures/earth_sphere10k.jpg", {.srgb = true});

        // Vertex vertices[] = {
        //     {{-0.5f, -0.5f, 0.f}, {1.f, 0.f, 0.f}, {0.f, 0.f}},
        //     {{+0.5f, -0.5f, 0.f}, {1.f, 1.f, 0.f}, {1.f, 0.f}},
        //     {{+0.5f, +0.5f, 0.f}, {0.f, 1.f, 0.f}, {1.f, 1.f}},
        //     {{-0.5f, +0.5f, 0.f}, {0.f, 0.f, 1.f}, {0.f, 1.f}},
        // };
        // unsigned int indices[] = {0, 1, 2, 2, 3, 0};

        Mesh mesh = create_sphere(16, 32);

        shader.use();
        texture.bind(0);
        shader.set_int("tex", 0);

        while (!glfwWindowShouldClose(window)) {
            glClearColor(BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, BG_COLOR.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            auto [_x, _y, width, height] = util::gl_get<float, 4>(GL_VIEWPORT);
            float aspect = width / height;
            glm::mat4 projection = glm::perspective(FOV, aspect, ZNEAR, ZFAR);
            // glm::mat4 projection = glm::ortho(-aspect, aspect, -1.f, 1.f, ZNEAR, ZFAR);
            shader.set_mat4("projection", projection);

            glm::mat4 modelview{1};
            modelview = glm::translate(modelview, glm::vec3(0, 0, -3));
            float angle = float(glfwGetTime()) * glm::pi<float>() / 4.f;
            modelview = glm::rotate(modelview, angle, glm::vec3(0, 1, 0));
            shader.set_mat4("modelview", modelview);

            mesh.draw();

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
