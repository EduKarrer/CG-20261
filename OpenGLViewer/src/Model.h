#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Mesh.h"
#include "Shader.h"

struct Material {
    glm::vec3 ambient  {0.1f, 0.1f, 0.1f};
    glm::vec3 diffuse  {0.8f, 0.8f, 0.8f};
    glm::vec3 specular {0.5f, 0.5f, 0.5f};
    float     shininess{32.0f};
    glm::vec3 color    {1.0f, 1.0f, 1.0f}; // tint
};

class Model {
public:
    std::string name;
    Material    material;
    bool        selected{false};

    // Transform state
    glm::vec3 position   {0.f};
    glm::vec3 rotationDeg{0.f};
    glm::vec3 scale      {1.f};

    explicit Model(const std::string& path);

    void draw(Shader& shader);

    glm::mat4 modelMatrix() const;

    std::vector<Mesh> meshes; // public so Scene can iterate for wireframe pass

private:
    std::string directory;

    void loadModel(const std::string& path);
    void processNode(struct aiNode* node, const struct aiScene* scene);
    Mesh processMesh(struct aiMesh* mesh, const struct aiScene* scene);
};
