#ifndef MODEL_H
#define MODEL_H

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "material.h"
#include "mesh.h"
#include "shader.h"

class Model {
  public:
    Model(std::vector<std::shared_ptr<Mesh>> meshes,
          std::vector<std::shared_ptr<Material>> materials = {})
        : meshes_(std::move(meshes)), materials_(std::move(materials)) {}
    void draw(const Shader* shader = nullptr) const;

    const std::vector<std::shared_ptr<Mesh>>& meshes() { return meshes_; }
    const std::vector<std::shared_ptr<Material>>& materials() { return materials_; }

  private:
    std::vector<std::shared_ptr<Mesh>> meshes_;
    std::vector<std::shared_ptr<Material>> materials_;
};

#endif // MODEL_H
