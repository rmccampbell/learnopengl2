#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"

struct Material {
    // Properties
    std::string name;
    float shininess = 0.0f;
    glm::vec3 diffuse_color = {1.0f, 1.0f, 1.0f};
    glm::vec3 specular_color = {0.0f, 0.0f, 0.0f};
    glm::vec3 ambient_color = {1.0f, 1.0f, 1.0f};
    glm::vec3 emissive_color = {0.0f, 0.0f, 0.0f};
    Texture diffuse_texture;
    Texture specular_texture;
    Texture ambient_texture;
    Texture emissive_texture;
    Texture ao_texture;
    Texture normal_texture;

    // Methods
    void apply(const Shader& shader) const;
};

std::ostream& operator<<(std::ostream& os, const Material& mat);

#endif // MATERIAL_H
