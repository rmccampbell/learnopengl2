#ifndef MESH_H
#define MESH_H

#include <memory>
#include <span>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "handle.h"
#include "material.h"
#include "shader.h"

using VaoHandle = Handle<GLuint, gl_delete_array_functor<glDeleteVertexArrays>>;
using BufferHandle = Handle<GLuint, gl_delete_array_functor<glDeleteBuffers>>;

enum Attr {
    POSITION = 0,
    NORMAL = 1,
    TEX_COORDS = 2,
};

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

class Mesh {
  public:
    Mesh(std::string_view name, std::span<const Vertex> vertices,
         std::span<const unsigned int> indices,
         std::shared_ptr<Material> material = nullptr);
    Mesh(std::span<const Vertex> vertices, std::span<const unsigned int> indices,
         std::shared_ptr<Material> material = nullptr)
        : Mesh("", vertices, indices, std::move(material)) {}
    void draw(const Shader* shader = nullptr) const;
    const std::string& name() const { return name_; }
    GLuint vao() const { return vao_.get(); };
    GLuint vbo() const { return vbo_.get(); };
    GLuint ebo() const { return ebo_.get(); };
    GLsizei num_indices() const { return num_indices_; };
    const std::shared_ptr<Material>& material() const { return material_; }
    void set_material(std::shared_ptr<Material> material) {
        material_ = std::move(material);
    }

  private:
    std::string name_;
    VaoHandle vao_;
    BufferHandle vbo_, ebo_;
    GLsizei num_indices_;
    std::shared_ptr<Material> material_;
};

#endif // MESH_H
