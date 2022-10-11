#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "common/assimp_loader.h"
#include "common/compat.h"
#include "common/errutils.h"
#include "common/glutils.h"
#include "common/lights.h"
#include "common/mesh.h"
#include "common/model.h"
#include "common/raii.h"
#include "common/shader.h"
#include "common/texture.h"

namespace fs = std::filesystem;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const float ZNEAR = 0.01f;
const float ZFAR = 100.f;
const float FOV = glm::radians(45.f);

const glm::vec4 BG_COLOR = {0.8f, 0.8f, 0.8f, 1.0f};

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

void run(const fs::path& exe_path) {
    fs::path root = exe_path.parent_path();

    err::check_glfw(glfwInit(), "failed to init GLFW: {}");
    ScopeGuardFn<glfwTerminate> guard;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window =
        glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    err::check_glfw(window, "failed to create GLFW window: {}");
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    err::check_glfw(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)),
                    "failed to load GL loader: {}");
    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    auto shader = Shader::load(root / "resources/shaders/shader.vs",
                               root / "resources/shaders/shader.fs");

    // TextureOpts opts{.srgb = true};
    // auto matl = std::make_shared<Material>();
    // matl->diffuse_texture = Texture(root / "resources/textures/earth_sphere10k.jpg", opts);
    // auto mesh = std::make_shared<Mesh>(create_sphere(16, 32));
    // mesh->set_material(matl);
    // Model model = Model({mesh}, {matl});

    Model model = load_model(root / "resources/models/nanosuit/nanosuit.obj");
    // Model model = load_model(R"(C:\Users\Ryan\3D Objects\Zelda\master_sword__hylian_shield\scene.gltf)");

    glm::mat4 modelmat{1};
    // modelmat = glm::scale(modelmat, glm::vec3(1.6f));
    modelmat = glm::translate(modelmat, {0, -1.5, 0});
    modelmat = glm::scale(modelmat, glm::vec3(1.f/5.f));
    // modelmat = glm::translate(modelmat, {0, -.5, -.5});
    // modelmat = glm::rotate(modelmat, glm::pi<float>(), {0, 0, 1});
    // modelmat = glm::rotate(modelmat, .5f*glm::pi<float>(), {1, 0, 0});
    // modelmat = glm::scale(modelmat, glm::vec3(1.f/100.f));

    shader.use();

    DirLight lights[] = {{.direction = {-1, -1, -1}}};
    apply_array(shader, "dirLights", "numDirLights", lights);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, BG_COLOR.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto [_x, _y, width, height] = util::gl_get<float, 4>(GL_VIEWPORT);
        float aspect = width / height;
        glm::mat4 projection = glm::perspective(FOV, aspect, ZNEAR, ZFAR);
        // glm::mat4 projection = glm::ortho(-aspect, aspect, -1.f, 1.f, ZNEAR, ZFAR);
        shader.set_mat4("projection", projection);

        glm::mat4 scenemat{1};
        scenemat = glm::translate(scenemat, {0, 0, -5});
        float angle = float(glfwGetTime()) * glm::pi<float>() / 4.f;
        scenemat = glm::rotate(scenemat, angle, {0, 1, 0});
        shader.set_mat4("model", scenemat * modelmat);
        shader.set_mat4("view", glm::mat4(1));

        model.draw(&shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int LGL_TMAIN(int argc, LGL_TCHAR* argv[]) {
    try {
        run(fs::canonical(argc ? argv[0] : fs::path()));
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
