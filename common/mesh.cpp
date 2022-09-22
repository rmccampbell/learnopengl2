#include "mesh.h"

#include <cstddef>
#include <utility>

Mesh::Mesh(std::string_view name, std::span<const Vertex> vertices,
           std::span<const unsigned int> indices, std::shared_ptr<Material> material)
    : name_(name), num_indices_(GLsizei(indices.size())), material_(std::move(material)) {
    glGenVertexArrays(1, &vao_.reset_as_ref());
    glGenBuffers(1, &vbo_.reset_as_ref());
    glGenBuffers(1, &ebo_.reset_as_ref());

    glBindVertexArray(*vao_);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(Attr::POSITION);
    glVertexAttribPointer(Attr::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(Attr::NORMAL);
    glVertexAttribPointer(Attr::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(Attr::TEX_COORDS);
    glVertexAttribPointer(Attr::TEX_COORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, tex_coords)));

    glBindVertexArray(0);
}

void Mesh::draw(const Shader* shader) const {
    if (material_ && shader) material_->apply(*shader);
    glBindVertexArray(*vao_);
    glDrawElements(GL_TRIANGLES, num_indices_, GL_UNSIGNED_INT, 0);
}
