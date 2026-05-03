#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {
public:
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;

    Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds);
    ~Mesh();

    void draw(Shader& shader) const;

private:
    GLuint VAO, VBO, EBO;
    void setup();
};
