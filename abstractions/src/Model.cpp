//
// Created by kairav on 6/7/26.
//

#include "../include/Model.h"


void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}

void Model::Draw(Shader& shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}

void Model::loadModel(const std::string& path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path,
        aiProcess_Triangulate        |
        aiProcess_FlipUVs            |
        aiProcess_CalcTangentSpace
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

std::vector<Texture> Model::loadMaterialTextures(
    aiMaterial* mat,
    aiTextureType type,
    const std::string& typeName,
    const aiScene* scene)
{
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            Texture texture;
            texture.id   = TextureFromFile(str.C_Str(), directory, scene);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    // ── Vertices ──────────────────────────────────────────────────────────────
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 v;

        v.x = mesh->mVertices[i].x;
        v.y = mesh->mVertices[i].y;
        v.z = mesh->mVertices[i].z;
        vertex.Position = v;

        if (mesh->HasNormals())
        {
            v.x = mesh->mNormals[i].x;
            v.y = mesh->mNormals[i].y;
            v.z = mesh->mNormals[i].z;
            vertex.Normal = v;
        }

        if (mesh->mTextureCoords[0])
        {
            vertex.TexCoords = { mesh->mTextureCoords[0][i].x,
                                 mesh->mTextureCoords[0][i].y };
            v.x = mesh->mTangents[i].x;
            v.y = mesh->mTangents[i].y;
            v.z = mesh->mTangents[i].z;
            vertex.Tangent = v;
            v.x = mesh->mBitangents[i].x;
            v.y = mesh->mBitangents[i].y;
            v.z = mesh->mBitangents[i].z;
            vertex.Bitangent = v;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    // ── Indices ───────────────────────────────────────────────────────────────
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // ── Textures ──────────────────────────────────────────────────────────────
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuseMaps =
            loadMaterialTextures(material, aiTextureType_BASE_COLOR, "texture_diffuse", scene);
        if (diffuseMaps.empty())
            diffuseMaps =
                loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
        if (diffuseMaps.empty())
            diffuseMaps =
                loadMaterialTextures(material, aiTextureType_UNKNOWN, "texture_diffuse", scene);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps =
            loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", scene);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        std::vector<Texture> normalMaps =
            loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", scene);
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        std::vector<Texture> heightMaps =
            loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", scene);
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }

    // ── Flat material color (used when no textures present) ───────────────────
    Mesh result(vertices, indices, textures);
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiColor4D diffuse(1.f, 1.f, 1.f, 1.f);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
        result.color = glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
    }
    return result;
}

Model::Model(const std::string& path)
{
    loadModel(path);
}

unsigned int TextureFromFile(
    const char* path,
    const std::string& directory,
    const aiScene* scene,
    bool gamma)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width = 0, height = 0, nrComponents = 0;
    unsigned char* data    = nullptr;
    bool           ownData = false;

    if (path[0] == '*')
    {
        int index = std::atoi(path + 1);
        const aiTexture* tex = scene->mTextures[index];

        if (tex->mHeight == 0)
        {
            data = stbi_load_from_memory(
                reinterpret_cast<unsigned char*>(tex->pcData),
                static_cast<int>(tex->mWidth),
                &width, &height, &nrComponents, 0
            );
        }
        else
        {
            width        = static_cast<int>(tex->mWidth);
            height       = static_cast<int>(tex->mHeight);
            nrComponents = 4;
            data         = new unsigned char[width * height * 4];
            ownData      = true;
            for (int i = 0; i < width * height; i++)
            {
                data[i*4+0] = tex->pcData[i].r;
                data[i*4+1] = tex->pcData[i].g;
                data[i*4+2] = tex->pcData[i].b;
                data[i*4+3] = tex->pcData[i].a;
            }
        }
    }
    else
    {
        std::string filename = directory + '/' + std::string(path);
        data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    }

    if (data)
    {
        GLenum format = (nrComponents == 1) ? GL_RED  :
                        (nrComponents == 3) ? GL_RGB  : GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (ownData) delete[] data;
        else         stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture: " << path << std::endl;
        if (!ownData && data) stbi_image_free(data);
    }

    return textureID;
}