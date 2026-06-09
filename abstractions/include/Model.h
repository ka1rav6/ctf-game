//
// Created by kairav on 6/7/26.
//

#ifndef MAIN_GAME_MODEL_H
#define MAIN_GAME_MODEL_H

#include "stb_image.h"
#include "Mesh.h"
#include "./common.h"
#include "./shader.h"

class Model
{
public:
    Model(const std::string& path);
    void Draw(Shader& shader);
    std::vector<Mesh> meshes;

private:
    std::string directory;
    std::vector<Texture> textures_loaded;

    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Texture> loadMaterialTextures(
        aiMaterial* mat,
        aiTextureType type,
        const std::string& typeName,
        const aiScene* scene   // needed for embedded texture lookup
    );
};

unsigned int TextureFromFile(
    const char* path,
    const std::string& directory,
    const aiScene* scene,
    bool gamma = false
);

#endif //MAIN_GAME_MODEL_H