//
// Created by kairav on 6/7/26.
//
#include "stb_image.h"

#ifndef MAIN_GAME_MODEL_H
#define MAIN_GAME_MODEL_H

#include "Mesh.h"
#include "./common.h"
#include "./shader.h"

class Model
{
public:
    Model(const std::string& path);

    void Draw(Shader& shader);

private:
    // model data
    std::vector<Mesh> meshes;
    std::string directory;

    std::vector<Texture> textures_loaded;

    // loading
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Texture> loadMaterialTextures(
        aiMaterial* mat,
        aiTextureType type,
        const std::string& typeName
    );
};

unsigned int TextureFromFile(
    const char* path,
    const std::string& directory,
    bool gamma = false
);

#endif //MAIN_GAME_MODEL_H
