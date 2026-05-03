#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Model.h"
#include "Shader.h"
#include "Camera.h"

struct PointLight {
    glm::vec3 position {2.f, 4.f, 2.f};
    glm::vec3 ambient  {0.2f};
    glm::vec3 diffuse  {1.f};
    glm::vec3 specular {1.f};
    float constant {1.f};
    float linear   {0.09f};
    float quadratic{0.032f};
};

class Scene {
public:
    Scene();

    void addModel(const std::string& path);
    void removeSelected();

    void draw(Shader& solidShader, Shader& wireShader);

    void selectNext();
    void selectPrev();
    int  selectedIndex() const { return m_selectedIdx; }

    Model* selected();

    std::vector<std::unique_ptr<Model>> models;
    PointLight light;
    bool wireframeOverlay{false};

private:
    int m_selectedIdx{0};

    void uploadLightUniforms(Shader& shader, const Camera& /*unused*/) {}
};
