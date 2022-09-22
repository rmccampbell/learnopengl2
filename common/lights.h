#ifndef LIGHTS_H
#define LIGHTS_H

#include <string>

#include <glm/glm.hpp>

#include "shader.h"

struct DirLight {
    glm::vec3 direction{0, -1, 0};

    glm::vec3 ambient{0};
    glm::vec3 diffuse{1};
    glm::vec3 specular = diffuse;

    void apply(const Shader& shader, const std::string& name) const;
};

struct PointLight {
    glm::vec3 position{0, 0, 0};

    float constant = 1;
    float linear = 0;
    float quadratic = 0;

    glm::vec3 ambient{0};
    glm::vec3 diffuse{1};
    glm::vec3 specular = diffuse;

    void apply(const Shader& shader, const std::string& name) const;
};

struct SpotLight {
    glm::vec3 position{0, 0, 0};
    glm::vec3 direction{0, -1, 0};
    float innerCutoff = 1;
    float outerCutoff = 0;

    float constant = 1;
    float linear = 0;
    float quadratic = 0;

    glm::vec3 ambient{0};
    glm::vec3 diffuse{1};
    glm::vec3 specular = diffuse;

    void apply(const Shader& shader, const std::string& name) const;
};

inline void DirLight::apply(const Shader& shader, const std::string& name) const {
    shader.set_vec3(name + ".direction", direction);
    shader.set_vec3(name + ".ambient", ambient);
    shader.set_vec3(name + ".diffuse", diffuse);
    shader.set_vec3(name + ".specular", specular);
}

inline void PointLight::apply(const Shader& shader, const std::string& name) const {
    shader.set_vec3(name + ".position", position);
    shader.set_float(name + ".constant", constant);
    shader.set_float(name + ".linear", linear);
    shader.set_float(name + ".quadratic", quadratic);
    shader.set_vec3(name + ".ambient", ambient);
    shader.set_vec3(name + ".diffuse", diffuse);
    shader.set_vec3(name + ".specular", specular);
}

inline void SpotLight::apply(const Shader& shader, const std::string& name) const {
    shader.set_vec3(name + ".position", position);
    shader.set_vec3(name + ".direction", direction);
    shader.set_float(name + ".inner_cutoff", innerCutoff);
    shader.set_float(name + ".outer_cutoff", outerCutoff);
    shader.set_float(name + ".constant", constant);
    shader.set_float(name + ".linear", linear);
    shader.set_float(name + ".quadratic", quadratic);
    shader.set_vec3(name + ".ambient", ambient);
    shader.set_vec3(name + ".diffuse", diffuse);
    shader.set_vec3(name + ".specular", specular);
}

#endif // LIGHTS_H
