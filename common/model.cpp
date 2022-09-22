#include "model.h"

void Model::draw(const Shader* shader) const {
    for (auto& mesh : meshes_) {
        mesh->draw(shader);
    }
}
