#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <glm/glm.hpp>

#include "mesh.h"

Mesh make_quad(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

Mesh make_cube();

Mesh make_sphere(int nlat, int nlon);

#endif // PRIMITIVES_H
