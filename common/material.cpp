#include "material.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/io.hpp>

namespace {

void apply_texture(const Texture& texture, const Shader& shader, const std::string& name,
                   unsigned int unit) {
    texture.bind(unit);
    shader.set_int(name + ".texture", unit);
    shader.set_bool(name + ".bound", !texture.empty());
}

} // namespace

void Material::apply(const Shader& shader) const {
    shader.set_float("material.shininess", shininess);
    shader.set_vec3("material.diffuse_color", diffuse_color);
    shader.set_vec3("material.specular_color", specular_color);
    shader.set_vec3("material.ambient_color", ambient_color);
    shader.set_vec3("material.emissive_color", emissive_color);

    apply_texture(diffuse_texture, shader, "material.diffuse_texture", 0);
    apply_texture(specular_texture, shader, "material.specular_texture", 1);
    apply_texture(ambient_texture, shader, "material.ambient_texture", 2);
    apply_texture(emissive_texture, shader, "material.emissive_texture", 3);
    apply_texture(ao_texture, shader, "material.ao_texture", 4);
    apply_texture(normal_texture, shader, "material.normal_texture", 5);
}

std::ostream& operator<<(std::ostream& os, const Material& mat) {
    os << "name: " << mat.name << '\n';
    os << "shininess: " << mat.shininess << '\n';
    os << "diffuse_color: " << mat.diffuse_color << '\n';
    os << "specular_color: " << mat.specular_color << '\n';
    os << "ambient_color: " << mat.ambient_color << '\n';
    os << "emissive_color: " << mat.emissive_color << '\n';
    os << "diffuse_texture: " << mat.diffuse_texture << '\n';
    os << "specular_texture: " << mat.specular_texture << '\n';
    os << "ambient_texture: " << mat.ambient_texture << '\n';
    os << "emissive_texture: " << mat.emissive_texture << '\n';
    os << "ao_texture: " << mat.ao_texture << '\n';
    os << "normal_texture: " << mat.normal_texture << '\n';
    return os;
}
