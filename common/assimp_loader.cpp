#include "assimp_loader.h"

#include <filesystem>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "errutils.h"
#include "mesh.h"
#include "model.h"
#include "u8tils.h"

namespace fs = std::filesystem;

namespace {

class ModelLoader {
  public:
    ModelLoader() {}

    Model load(const fs::path& path, unsigned int flags) {
        materials.clear();
        meshes.clear();

        Assimp::Importer importer;
        flags |= aiProcess_PreTransformVertices;
        const aiScene* scene = importer.ReadFile(u8::path_to_char(path), flags);
        err::check(scene && !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE),
                   "assimp error: {}", importer.GetErrorString());

        fs::path directory = path.parent_path();
        materials.reserve(scene->mNumMaterials);
        for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
            materials.push_back(convert_material(scene->mMaterials[i], directory));
        }

        meshes.reserve(scene->mNumMeshes);
        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            meshes.push_back(convert_mesh(scene->mMeshes[i]));
        }

        return Model(std::move(meshes), std::move(materials));
    }

  private:
    static void get_color(aiMaterial* mat, const char* pKey, unsigned int type,
                          unsigned int index, glm::vec3& out) {
        aiColor3D color;
        if (mat->Get(pKey, type, index, color) == AI_SUCCESS) {
            out = {color.r, color.g, color.b};
        }
    }

    static Texture get_texture(aiMaterial* mat, const fs::path& directory,
                               aiTextureType type, unsigned int index = 0) {
        aiString ai_path;
        if (mat->GetTexture(type, index, &ai_path) != AI_SUCCESS)
            return Texture();
        fs::path path = directory / u8::to_path(ai_path.C_Str());
        // Assume relative filename if wrong path is hard-coded
        if (!fs::exists(path))
            path = directory / path.filename();
        return Texture(path);
    }

    std::shared_ptr<Material> convert_material(aiMaterial* mat,
                                               const fs::path& directory) {
        auto res = std::make_shared<Material>();
        aiString name;
        mat->Get(AI_MATKEY_NAME, name);
        res->name = name.C_Str();

        mat->Get(AI_MATKEY_SHININESS, res->shininess);

        get_color(mat, AI_MATKEY_COLOR_DIFFUSE, res->diffuse_color);
        get_color(mat, AI_MATKEY_COLOR_SPECULAR, res->specular_color);
        res->ambient_color = res->diffuse_color;
        get_color(mat, AI_MATKEY_COLOR_AMBIENT, res->ambient_color);
        get_color(mat, AI_MATKEY_COLOR_EMISSIVE, res->emissive_color);

        res->diffuse_texture = get_texture(mat, directory, aiTextureType_DIFFUSE);
        res->specular_texture = get_texture(mat, directory, aiTextureType_SPECULAR);
        res->ambient_texture = get_texture(mat, directory, aiTextureType_AMBIENT);
        res->emissive_texture = get_texture(mat, directory, aiTextureType_EMISSIVE);
        res->ao_texture = get_texture(mat, directory, aiTextureType_LIGHTMAP);
        res->normal_texture = get_texture(mat, directory, aiTextureType_NORMALS);
        return res;
    }

    std::shared_ptr<Mesh> convert_mesh(aiMesh* mesh) {
        std::vector<Vertex> vertices;
        for (size_t i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex{};
            aiVector3D& v = mesh->mVertices[i];
            vertex.position = {v.x, v.y, v.z};
            aiVector3D& n = mesh->mNormals[i];
            vertex.normal = {n.x, n.y, n.z};
            if (mesh->HasTextureCoords(0)) {
                aiVector3D& tc = mesh->mTextureCoords[0][i];
                vertex.tex_coords = {tc.x, tc.y};
            }
            vertices.push_back(vertex);
        }

        std::vector<unsigned int> indices;
        for (size_t i = 0; i < mesh->mNumFaces; i++) {
            aiFace& face = mesh->mFaces[i];
            for (size_t j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        std::string_view name = mesh->mName.C_Str();
        auto material = materials[mesh->mMaterialIndex];
        return std::make_shared<Mesh>(name, vertices, indices, std::move(material));
    }

    std::vector<std::shared_ptr<Material>> materials;
    std::vector<std::shared_ptr<Mesh>> meshes;
};

} // namespace

Model load_model(const fs::path& path, unsigned int flags) {
    return ModelLoader().load(path, flags);
}
