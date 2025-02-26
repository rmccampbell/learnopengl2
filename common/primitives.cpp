#include "primitives.h"

#include <array>
#include <vector>

#include "utils.h"

template <unsigned int N>
constexpr std::array<unsigned int, 6 * N> quad_indices() {
    std::array<unsigned int, 6 * N> indices;
    for (unsigned int i = 0; i < N; i++) {
        indices[6 * i + 0] = 4 * i + 0;
        indices[6 * i + 1] = 4 * i + 1;
        indices[6 * i + 2] = 4 * i + 2;
        indices[6 * i + 3] = 4 * i + 2;
        indices[6 * i + 4] = 4 * i + 3;
        indices[6 * i + 5] = 4 * i + 0;
    }
    return indices;
}

std::vector<unsigned int> quad_indices(unsigned int n) {
    std::vector<unsigned int> indices;
    indices.reserve(6*n);
    for (unsigned int i = 0; i < n; i++) {
        indices.push_back(4 * i + 0);
        indices.push_back(4 * i + 1);
        indices.push_back(4 * i + 2);
        indices.push_back(4 * i + 2);
        indices.push_back(4 * i + 3);
        indices.push_back(4 * i + 0);
    }
    return indices;
}

std::vector<unsigned int> quad_grid_indices(unsigned int m, unsigned int n) {
    std::vector<unsigned int> indices;
    indices.reserve(6 * m * n);
    unsigned int rowlen = n + 1;
    for (unsigned int i = 0; i < m; i++) {
        for (unsigned int j = 0; j < n; j++) {
            indices.push_back(rowlen*(i + 0) + (j + 0));
            indices.push_back(rowlen*(i + 0) + (j + 1));
            indices.push_back(rowlen*(i + 1) + (j + 1));
            indices.push_back(rowlen*(i + 1) + (j + 1));
            indices.push_back(rowlen*(i + 1) + (j + 0));
            indices.push_back(rowlen*(i + 0) + (j + 0));
        }
    }
    return indices;
}

glm::vec3 tri_normal(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2) {
    return glm::normalize(glm::cross(p1 - p0, p2 - p0));
}

Mesh make_quad(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
    glm::vec3 normal = tri_normal(p0, p1, p3);
    std::array<Vertex, 4> vertices = {{
        {p0, normal, {0, 0}},
        {p1, normal, {1, 0}},
        {p2, normal, {1, 1}},
        {p3, normal, {0, 1}},
    }};
    constexpr auto indices = quad_indices<1>();
    return Mesh(vertices, indices);
}

Mesh make_smooth_quad(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
    std::array<Vertex, 4> vertices = {{
        {p0, tri_normal(p0, p1, p3), {0, 0}},
        {p1, tri_normal(p1, p2, p0), {1, 0}},
        {p2, tri_normal(p2, p3, p1), {1, 1}},
        {p3, tri_normal(p3, p0, p2), {0, 1}},
    }};
    constexpr auto indices = quad_indices<1>();
    return Mesh(vertices, indices);
}

constexpr glm::vec3 mul(const glm::mat3& m, const glm::vec3& v) {
    return {m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z,
            m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z,
            m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z};
}

// orient: {face x axis, face y axis, face normal}
// tex_off: integer offset in cube net from bottom left
constexpr std::array<Vertex, 4> cube_face(const glm::mat3& orient, glm::vec2 tex_off) {
    constexpr glm::vec2 tex_size = {4, 3};
    glm::vec3 normal = orient[2];
    std::array<Vertex, 4> verts;
    for (int i = 0; i < 4; i++) {
        glm::vec2 coord = {(i + 1) % 4 / 2, i / 2};
        glm::vec3 position = mul(orient, glm::vec3(coord, 1) - .5f);
        glm::vec2 tex_coords = (tex_off + coord) / tex_size;
        verts[i] = {position, normal, tex_coords};
    }
    return verts;
};

Mesh make_cube() {
    constexpr std::array<Vertex, 6*4> vertices = util::array_cat(
        cube_face({{-1, 0, +0}, {0, 1, +0}, {+0, +0, -1}}, {3, 1}), // back
        cube_face({{+1, 0, +0}, {0, 1, +0}, {+0, +0, +1}}, {1, 1}), // front
        cube_face({{+0, 0, +1}, {0, 1, +0}, {-1, +0, +0}}, {0, 1}), // left
        cube_face({{+0, 0, -1}, {0, 1, +0}, {+1, +0, +0}}, {2, 1}), // right
        cube_face({{+1, 0, +0}, {0, 0, +1}, {+0, -1, +0}}, {1, 0}), // bottom
        cube_face({{+1, 0, +0}, {0, 0, -1}, {+0, +1, +0}}, {1, 2})  // top
    );
    constexpr auto indices = quad_indices<6>();
    return Mesh(vertices, indices);
}

Mesh make_cube_hardcoded() {
    constexpr float f1_3 = 1.0f / 3.0f, f2_3 = 2.0f / 3.0f;
    constexpr std::array<Vertex, 24> vertices = {{
        // Back
        {{+0.5f, -0.5f, -0.5f}, {+0.0f, +0.0f, -1.0f}, {0.75f, f1_3}},
        {{-0.5f, -0.5f, -0.5f}, {+0.0f, +0.0f, -1.0f}, {1.00f, f1_3}},
        {{-0.5f, +0.5f, -0.5f}, {+0.0f, +0.0f, -1.0f}, {1.00f, f2_3}},
        {{+0.5f, +0.5f, -0.5f}, {+0.0f, +0.0f, -1.0f}, {0.75f, f2_3}},
        // Front
        {{-0.5f, -0.5f, +0.5f}, {+0.0f, +0.0f, +1.0f}, {0.25f, f1_3}},
        {{+0.5f, -0.5f, +0.5f}, {+0.0f, +0.0f, +1.0f}, {0.50f, f1_3}},
        {{+0.5f, +0.5f, +0.5f}, {+0.0f, +0.0f, +1.0f}, {0.50f, f2_3}},
        {{-0.5f, +0.5f, +0.5f}, {+0.0f, +0.0f, +1.0f}, {0.25f, f2_3}},
        // Left
        {{-0.5f, -0.5f, -0.5f}, {-1.0f, +0.0f, +0.0f}, {0.00f, f1_3}},
        {{-0.5f, -0.5f, +0.5f}, {-1.0f, +0.0f, +0.0f}, {0.25f, f1_3}},
        {{-0.5f, +0.5f, +0.5f}, {-1.0f, +0.0f, +0.0f}, {0.25f, f2_3}},
        {{-0.5f, +0.5f, -0.5f}, {-1.0f, +0.0f, +0.0f}, {0.00f, f2_3}},
        // Right
        {{+0.5f, -0.5f, +0.5f}, {+1.0f, +0.0f, +0.0f}, {0.50f, f1_3}},
        {{+0.5f, -0.5f, -0.5f}, {+1.0f, +0.0f, +0.0f}, {0.75f, f1_3}},
        {{+0.5f, +0.5f, -0.5f}, {+1.0f, +0.0f, +0.0f}, {0.75f, f2_3}},
        {{+0.5f, +0.5f, +0.5f}, {+1.0f, +0.0f, +0.0f}, {0.50f, f2_3}},
        // Bottom
        {{-0.5f, -0.5f, -0.5f}, {+0.0f, -1.0f, +0.0f}, {0.25f, 0.0f}},
        {{+0.5f, -0.5f, -0.5f}, {+0.0f, -1.0f, +0.0f}, {0.50f, 0.0f}},
        {{+0.5f, -0.5f, +0.5f}, {+0.0f, -1.0f, +0.0f}, {0.50f, f1_3}},
        {{-0.5f, -0.5f, +0.5f}, {+0.0f, -1.0f, +0.0f}, {0.25f, f1_3}},
        // Top
        {{-0.5f, +0.5f, +0.5f}, {+0.0f, +1.0f, +0.0f}, {0.25f, f2_3}},
        {{+0.5f, +0.5f, +0.5f}, {+0.0f, +1.0f, +0.0f}, {0.50f, f2_3}},
        {{+0.5f, +0.5f, -0.5f}, {+0.0f, +1.0f, +0.0f}, {0.50f, 1.0f}},
        {{-0.5f, +0.5f, -0.5f}, {+0.0f, +1.0f, +0.0f}, {0.25f, 1.0f}},
    }};
    constexpr auto indices = quad_indices<6>();
    return Mesh(vertices, indices);
}

Mesh make_sphere(int nlat, int nlon) {
    std::vector<Vertex> vertices;
    vertices.reserve((nlat + 1) * (nlon + 1));
    for (int i = 0; i <= nlat; i++) {
        for (int j = 0; j <= nlon; j++) {
            float theta = float(j % nlon) / nlon * glm::two_pi<float>();
            float phi = float(i) / nlat * glm::pi<float>();
            float x = glm::cos(theta) * glm::sin(phi);
            float y = -glm::cos(phi);
            float z = -glm::sin(theta) * glm::sin(phi);
            float u = float(j) / nlon;
            float v = float(i) / nlat;
            vertices.push_back(
                Vertex{.position = {x, y, z}, .normal = {x, y, z}, .tex_coords = {u, v}});
        }
    }
    auto indices = quad_grid_indices(nlat, nlon);
    return Mesh(vertices, indices);
}
