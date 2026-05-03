#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <sstream>

#include "Shader.h"
#include "Camera.h"
#include "Scene.h"

// ── Window settings ────────────────────────────────────────────────────────────
constexpr int SCR_W = 1280;
constexpr int SCR_H = 720;

// ── Globals ────────────────────────────────────────────────────────────────────
Camera  camera({0.f, 2.f, 8.f});
Scene   scene;

float lastX = SCR_W / 2.f, lastY = SCR_H / 2.f;
bool  firstMouse = true;
bool  mouseCaptured = true;

float deltaTime = 0.f;
float lastFrame = 0.f;

// Debounce keys
bool tabPressed     = false;
bool wirePressed    = false;
bool projPressed    = false;

// ── GLFW callbacks ─────────────────────────────────────────────────────────────
void framebuffer_size_callback(GLFWwindow*, int w, int h) {
    glViewport(0, 0, w, h);
}

void mouse_callback(GLFWwindow*, double xpos, double ypos) {
    if (!mouseCaptured) return;
    if (firstMouse) { lastX = (float)xpos; lastY = (float)ypos; firstMouse = false; }
    float xoff = (float)xpos - lastX;
    float yoff = lastY - (float)ypos;   // y inverted
    lastX = (float)xpos; lastY = (float)ypos;
    camera.processMouseMovement(xoff, yoff);
}

void scroll_callback(GLFWwindow*, double, double yoff) {
    camera.processScroll((float)yoff);
}

void key_callback(GLFWwindow* window, int key, int, int action, int) {
    // ESC – toggle mouse capture
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        mouseCaptured = !mouseCaptured;
        if (mouseCaptured) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            firstMouse = true;
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    // Tab – next object
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS && !tabPressed) {
        scene.selectNext(); tabPressed = true;
    }
    if (key == GLFW_KEY_TAB && action == GLFW_RELEASE) tabPressed = false;

    // Q – prev object
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        scene.selectPrev();

    // F – wireframe overlay
    if (key == GLFW_KEY_F && action == GLFW_PRESS && !wirePressed) {
        scene.wireframeOverlay = !scene.wireframeOverlay; wirePressed = true;
    }
    if (key == GLFW_KEY_F && action == GLFW_RELEASE) wirePressed = false;

    // P – toggle projection
    if (key == GLFW_KEY_P && action == GLFW_PRESS && !projPressed) {
        camera.toggleProjection(); projPressed = true;
        std::cout << "[Cam] Projection: "
                  << (camera.projMode == ProjectionMode::Perspective ? "Perspective" : "Orthographic")
                  << "\n";
    }
    if (key == GLFW_KEY_P && action == GLFW_RELEASE) projPressed = false;
}

// ── Per-frame input ────────────────────────────────────────────────────────────
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W)     == GLFW_PRESS) camera.processKeyboard(FORWARD,   deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S)     == GLFW_PRESS) camera.processKeyboard(BACKWARD,  deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A)     == GLFW_PRESS) camera.processKeyboard(LEFT,       deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D)     == GLFW_PRESS) camera.processKeyboard(RIGHT_DIR,  deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.processKeyboard(UP_DIR,     deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) camera.processKeyboard(DOWN_DIR, deltaTime);

    Model* sel = scene.selected();
    if (!sel) return;

    float tSpeed = 2.0f * deltaTime;
    float rSpeed = 60.f * deltaTime;
    float sSpeed = 1.0f * deltaTime;

    // Rotation – numpad or IJKLUO
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) sel->rotationDeg.x += rSpeed;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) sel->rotationDeg.x -= rSpeed;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) sel->rotationDeg.y += rSpeed;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) sel->rotationDeg.y -= rSpeed;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) sel->rotationDeg.z += rSpeed;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) sel->rotationDeg.z -= rSpeed;

    // Translation – arrow keys + R/Y
    if (glfwGetKey(window, GLFW_KEY_LEFT)  == GLFW_PRESS) sel->position.x -= tSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) sel->position.x += tSpeed;
    if (glfwGetKey(window, GLFW_KEY_UP)    == GLFW_PRESS) sel->position.z -= tSpeed;
    if (glfwGetKey(window, GLFW_KEY_DOWN)  == GLFW_PRESS) sel->position.z += tSpeed;
    if (glfwGetKey(window, GLFW_KEY_R)     == GLFW_PRESS) sel->position.y += tSpeed;
    if (glfwGetKey(window, GLFW_KEY_Y)     == GLFW_PRESS) sel->position.y -= tSpeed;

    // Scale – +/-
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) sel->scale += sSpeed; // +
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
        sel->scale -= sSpeed;
        if (sel->scale.x < 0.05f) sel->scale = {0.05f,0.05f,0.05f};
    }
}

// ── Grid drawing ──────────────────────────────────────────────────────────────
GLuint gridVAO = 0, gridVBO = 0;
int    gridLineCount = 0;

void buildGrid(float size, int divisions) {
    std::vector<float> verts;
    float step = size * 2.f / (float)divisions;
    for (int i = 0; i <= divisions; i++) {
        float t = -size + i * step;
        // parallel to Z
        verts.insert(verts.end(), {t, 0.f, -size,   t, 0.f,  size});
        // parallel to X
        verts.insert(verts.end(), {-size, 0.f, t,    size, 0.f, t});
    }
    gridLineCount = (int)verts.size() / 3;

    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);
    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glBindVertexArray(0);
}

// ── Print help ─────────────────────────────────────────────────────────────────
void printHelp() {
    std::cout << R"(
╔══════════════════════════════════════════════════════════╗
║       OpenGL 3D Scene Viewer – GRAU A                    ║
╠══════════════════════════════════════════════════════════╣
║ CAMERA                                                   ║
║  W/A/S/D        – move forward/left/back/right           ║
║  SPACE/CTRL     – move up / down                         ║
║  Mouse          – look around (click window to capture)  ║
║  Scroll         – zoom                                   ║
║  ESC            – toggle mouse capture                   ║
║  P              – toggle Perspective / Orthographic      ║
╠══════════════════════════════════════════════════════════╣
║ OBJECT SELECTION                                         ║
║  TAB            – select next object                     ║
║  Q              – select previous object                 ║
╠══════════════════════════════════════════════════════════╣
║ TRANSFORM (selected object)                              ║
║  I/K            – rotate X axis                          ║
║  J/L            – rotate Y axis                          ║
║  U/O            – rotate Z axis                          ║
║  Arrow keys     – translate X/Z                          ║
║  R/Y            – translate Y up/down                    ║
║  +/-            – scale uniform                          ║
╠══════════════════════════════════════════════════════════╣
║ DISPLAY                                                  ║
║  F              – toggle wireframe overlay               ║
╚══════════════════════════════════════════════════════════╝
)";
}

// ── Main ──────────────────────────────────────────────────────────────────────
int main(int argc, char** argv) {
    // ── GLFW init ──────────────────────────────────────────────────────────────
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_W, SCR_H, "OpenGL 3D Viewer – Grau A", nullptr, nullptr);
    if (!window) { std::cerr << "GLFW window creation failed\n"; glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // ── GLAD ───────────────────────────────────────────────────────────────────
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n"; return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // ── Shaders ────────────────────────────────────────────────────────────────
    Shader solidShader("shaders/phong.vert", "shaders/phong.frag");
    Shader wireShader ("shaders/wire.vert",  "shaders/wire.frag");
    Shader gridShader ("shaders/grid.vert",  "shaders/grid.frag");

    // ── Grid ───────────────────────────────────────────────────────────────────
    buildGrid(20.f, 40);

    // ── Load models from CLI args or defaults ──────────────────────────────────
    if (argc > 1) {
        for (int i = 1; i < argc; i++)
            scene.addModel(argv[i]);
    } else {
        // Default: 3 colored cubes
        scene.addModel("assets/models/cube.obj");
        scene.addModel("assets/models/cube.obj");
        scene.addModel("assets/models/cube.obj");
        if (!scene.models.empty()) {
            scene.models[0]->material.color = {1.f, 0.2f, 0.2f};
            if (scene.models.size() > 1)
                scene.models[1]->material.color = {0.2f, 1.f, 0.2f};
        }
    }

    printHelp();

    // ── Render loop ────────────────────────────────────────────────────────────
    while (!glfwWindowShouldClose(window)) {
        float now = (float)glfwGetTime();
        deltaTime = now - lastFrame;
        lastFrame = now;

        processInput(window);

        // Clear
        glClearColor(0.12f, 0.12f, 0.16f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Matrices
        int fw, fh;
        glfwGetFramebufferSize(window, &fw, &fh);
        float aspect = (fh > 0) ? (float)fw / (float)fh : 1.f;
        glm::mat4 view = camera.getView();
        glm::mat4 proj = camera.getProjection(aspect);

        // Grid
        gridShader.use();
        gridShader.setMat4("view", view);
        gridShader.setMat4("projection", proj);
        gridShader.setMat4("model", glm::mat4(1.f));
        glBindVertexArray(gridVAO);
        glDrawArrays(GL_LINES, 0, gridLineCount);
        glBindVertexArray(0);

        // Scene
        solidShader.use();
        solidShader.setMat4 ("view",       view);
        solidShader.setMat4 ("projection", proj);
        solidShader.setVec3 ("viewPos",    camera.Position);

        wireShader.use();
        wireShader.setMat4("view",       view);
        wireShader.setMat4("projection", proj);

        scene.draw(solidShader, wireShader);

        // Title with info
        std::ostringstream title;
        title << "OpenGL 3D Viewer";
        if (scene.selected())
            title << "  |  Object: " << scene.selected()->name
                  << "  [" << scene.selectedIndex()+1 << "/" << scene.models.size() << "]";
        title << "  |  "
              << (camera.projMode == ProjectionMode::Perspective ? "Perspective" : "Orthographic");
        glfwSetWindowTitle(window, title.str().c_str());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &gridVAO);
    glDeleteBuffers(1, &gridVBO);
    glfwTerminate();
    return 0;
}
