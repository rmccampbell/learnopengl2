#ifndef MESH_H
#define MESH_H

#include <span>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "handle.h"

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
    Mesh(std::span<Vertex> vertices, std::span<unsigned int> indices);
    void draw();
    GLuint vao() { return vao_; };
    GLuint vbo() { return vbo_; };
    GLuint ebo() { return ebo_; };
    GLsizei num_indices() { return num_indices_; };
private:
    VaoHandle vao_;
    BufferHandle vbo_, ebo_;
    GLsizei num_indices_;
};

#endif // MESH_H
