#include "Scene.h"
#include <stdexcept>
#include <iostream>

Scene::Scene() {}

void Scene::addModel(const std::string& path) {
    try {
        models.push_back(std::make_unique<Model>(path));
        // Offset each new model slightly so they don't overlap
        float offset = (float)(models.size() - 1) * 2.5f;
        models.back()->position.x = offset;
        m_selectedIdx = (int)models.size() - 1;
        // Mark selected
        for (int i = 0; i < (int)models.size(); i++)
            models[i]->selected = (i == m_selectedIdx);
        std::cout << "[Scene] Loaded: " << path << "\n";
    } catch (const std::exception& e) {
        std::cerr << "[Scene] Failed to load " << path << ": " << e.what() << "\n";
    }
}

void Scene::removeSelected() {
    if (models.empty()) return;
    models.erase(models.begin() + m_selectedIdx);
    m_selectedIdx = std::max(0, m_selectedIdx - 1);
    for (int i = 0; i < (int)models.size(); i++)
        models[i]->selected = (i == m_selectedIdx);
}

Model* Scene::selected() {
    if (models.empty()) return nullptr;
    return models[m_selectedIdx].get();
}

void Scene::selectNext() {
    if (models.empty()) return;
    models[m_selectedIdx]->selected = false;
    m_selectedIdx = (m_selectedIdx + 1) % (int)models.size();
    models[m_selectedIdx]->selected = true;
}

void Scene::selectPrev() {
    if (models.empty()) return;
    models[m_selectedIdx]->selected = false;
    m_selectedIdx = ((m_selectedIdx - 1) + (int)models.size()) % (int)models.size();
    models[m_selectedIdx]->selected = true;
}

void Scene::draw(Shader& solidShader, Shader& wireShader) {
    // Upload light to solid shader
    solidShader.use();
    solidShader.setVec3 ("light.position",  light.position);
    solidShader.setVec3 ("light.ambient",   light.ambient);
    solidShader.setVec3 ("light.diffuse",   light.diffuse);
    solidShader.setVec3 ("light.specular",  light.specular);
    solidShader.setFloat("light.constant",  light.constant);
    solidShader.setFloat("light.linear",    light.linear);
    solidShader.setFloat("light.quadratic", light.quadratic);

    // Draw solid
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for (auto& m : models)
        m->draw(solidShader);

    // Wireframe overlay
    if (wireframeOverlay) {
        wireShader.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(1.0f);
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-1.f, -1.f);
        for (auto& m : models) {
            wireShader.setMat4("model", m->modelMatrix());
            wireShader.setBool("uSelected", m->selected);
            for (auto& mesh : m->meshes)
                mesh.draw(wireShader);
        }
        glDisable(GL_POLYGON_OFFSET_LINE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
