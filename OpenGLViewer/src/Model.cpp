#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdexcept>

Model::Model(const std::string& path) {
    name = path.substr(path.find_last_of("/\\") + 1);
    loadModel(path);
}

void Model::draw(Shader& shader) {
    // Upload material uniforms
    shader.setVec3 ("material.ambient",   material.ambient);
    shader.setVec3 ("material.diffuse",   material.diffuse);
    shader.setVec3 ("material.specular",  material.specular);
    shader.setFloat("material.shininess", material.shininess);
    shader.setVec3 ("material.color",     material.color);
    shader.setBool ("uSelected",          selected);
    shader.setMat4 ("model",              modelMatrix());

    for (auto& mesh : meshes)
        mesh.draw(shader);
}

glm::mat4 Model::modelMatrix() const {
    glm::mat4 m{1.f};
    m = glm::translate(m, position);
    m = glm::rotate(m, glm::radians(rotationDeg.x), {1,0,0});
    m = glm::rotate(m, glm::radians(rotationDeg.y), {0,1,0});
    m = glm::rotate(m, glm::radians(rotationDeg.z), {0,0,1});
    m = glm::scale(m, scale);
    return m;
}

void Model::loadModel(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs |
        aiProcess_CalcTangentSpace
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error(std::string("Assimp error: ") + importer.GetErrorString());
    }

    directory = path.substr(0, path.find_last_of("/\\"));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    for (unsigned i = 0; i < node->mNumMeshes; i++)
        meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene));
    for (unsigned i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;

    vertices.reserve(mesh->mNumVertices);
    for (unsigned i = 0; i < mesh->mNumVertices; i++) {
        Vertex v;
        v.Position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};

        if (mesh->HasNormals())
            v.Normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};

        if (mesh->mTextureCoords[0])
            v.TexCoords = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
        else
            v.TexCoords = {0.f, 0.f};

        vertices.push_back(v);
    }

    for (unsigned i = 0; i < mesh->mNumFaces; i++) {
        aiFace& face = mesh->mFaces[i];
        for (unsigned j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    return Mesh(std::move(vertices), std::move(indices));
}
