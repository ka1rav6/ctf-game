//
// Created by kairav on 6/7/26.
//

#ifndef MAIN_GAME_MODEL_H
#define MAIN_GAME_MODEL_H

#include "Mesh.h"
#include "./common.h"
#include "./shader.h"

class Model
{`
public:
    Model(char *path)
    {
        loadModel(path);
    }
    void Draw(Shader &shader);
private:
    // model data
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;
    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                         std::string typeName);
};

#endif //MAIN_GAME_MODEL_H
