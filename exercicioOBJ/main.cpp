#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

// ─── Shader sources ────────────────────────────────────────────────────────────

const char* vertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragNormal;
out vec3 FragPos;

void main() {
    FragPos    = vec3(model * vec4(aPos, 1.0));
    FragNormal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
)";

const char* fragmentShaderSrc = R"(
#version 330 core
in vec3 FragNormal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform bool wireframe;

out vec4 FragColor;

void main() {
    if (wireframe) {
        FragColor = vec4(objectColor * 0.4, 1.0);
        return;
    }
    // Simple diffuse + ambient
    vec3 lightDir = normalize(vec3(1.0, 1.5, 2.0));
    vec3 norm = normalize(FragNormal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 ambient = 0.25 * objectColor;
    vec3 diffuse = diff * objectColor;
    FragColor = vec4(ambient + diffuse, 1.0);
}
)";

// ─── OBJ Model ─────────────────────────────────────────────────────────────────

struct SceneObject {
    // GPU data
    GLuint VAO = 0, VBO = 0;
    int    vertexCount = 0;

    // Transform state
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);   // degrees
    glm::vec3 scale    = glm::vec3(1.0f);

    // Visual
    glm::vec3 color;
    std::string name;

    glm::mat4 modelMatrix() const {
        glm::mat4 m = glm::mat4(1.0f);
        m = glm::translate(m, position);
        m = glm::rotate(m, glm::radians(rotation.x), glm::vec3(1,0,0));
        m = glm::rotate(m, glm::radians(rotation.y), glm::vec3(0,1,0));
        m = glm::rotate(m, glm::radians(rotation.z), glm::vec3(0,0,1));
        m = glm::scale(m, scale);
        return m;
    }
};

// ─── OBJ Loader ────────────────────────────────────────────────────────────────

bool loadOBJ(const std::string& path, SceneObject& obj) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open: " << path << std::endl;
        return false;
    }

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<float>     vertices; // interleaved pos+normal

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string token;
        ss >> token;

        if (token == "v") {
            glm::vec3 p;
            ss >> p.x >> p.y >> p.z;
            positions.push_back(p);
        } else if (token == "vn") {
            glm::vec3 n;
            ss >> n.x >> n.y >> n.z;
            normals.push_back(n);
        } else if (token == "f") {
            // Support: f v//vn  or  f v/vt/vn  or  f v
            std::vector<int> pIdx, nIdx;
            std::string vert;
            while (ss >> vert) {
                // replace '/' with spaces
                for (char& c : vert) if (c == '/') c = ' ';
                std::istringstream vs(vert);
                int pi = 0, ti = 0, ni = 0;
                vs >> pi;
                std::string rest;
                if (vs >> ti) { if (!(vs >> ni)) ni = 0; }
                pIdx.push_back(pi - 1);
                nIdx.push_back(ni - 1);
            }
            // Triangulate polygon (fan)
            for (int i = 1; i + 1 < (int)pIdx.size(); ++i) {
                int tri[3] = {0, i, i+1};
                for (int t : tri) {
                    glm::vec3 p = positions[pIdx[t]];
                    vertices.push_back(p.x);
                    vertices.push_back(p.y);
                    vertices.push_back(p.z);
                    if (!normals.empty() && nIdx[t] >= 0 && nIdx[t] < (int)normals.size()) {
                        glm::vec3 n = normals[nIdx[t]];
                        vertices.push_back(n.x);
                        vertices.push_back(n.y);
                        vertices.push_back(n.z);
                    } else {
                        vertices.push_back(0); vertices.push_back(1); vertices.push_back(0);
                    }
                }
            }
        }
    }

    if (vertices.empty()) return false;

    // Normalize model to fit in [-1,1]
    float minX=1e9,minY=1e9,minZ=1e9,maxX=-1e9,maxY=-1e9,maxZ=-1e9;
    for (int i = 0; i < (int)vertices.size(); i += 6) {
        minX=std::min(minX,vertices[i]);   maxX=std::max(maxX,vertices[i]);
        minY=std::min(minY,vertices[i+1]); maxY=std::max(maxY,vertices[i+1]);
        minZ=std::min(minZ,vertices[i+2]); maxZ=std::max(maxZ,vertices[i+2]);
    }
    float cx=(minX+maxX)/2, cy=(minY+maxY)/2, cz=(minZ+maxZ)/2;
    float ext=std::max({maxX-minX, maxY-minY, maxZ-minZ})/2.0f;
    if (ext < 1e-6f) ext = 1.0f;
    for (int i = 0; i < (int)vertices.size(); i += 6) {
        vertices[i]   = (vertices[i]   - cx) / ext;
        vertices[i+1] = (vertices[i+1] - cy) / ext;
        vertices[i+2] = (vertices[i+2] - cz) / ext;
    }

    glGenVertexArrays(1, &obj.VAO);
    glGenBuffers(1, &obj.VBO);
    glBindVertexArray(obj.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    obj.vertexCount = (int)vertices.size() / 6;
    return true;
}

// ─── Procedural fallback: generate a simple cube ───────────────────────────────

void generateCube(SceneObject& obj) {
    // 36 vertices for a cube with normals
    float verts[] = {
        // pos              normal
        -1,-1,-1,  0, 0,-1,   1,-1,-1,  0, 0,-1,   1, 1,-1,  0, 0,-1,
         1, 1,-1,  0, 0,-1,  -1, 1,-1,  0, 0,-1,  -1,-1,-1,  0, 0,-1,
        -1,-1, 1,  0, 0, 1,   1,-1, 1,  0, 0, 1,   1, 1, 1,  0, 0, 1,
         1, 1, 1,  0, 0, 1,  -1, 1, 1,  0, 0, 1,  -1,-1, 1,  0, 0, 1,
        -1, 1, 1, -1, 0, 0,  -1, 1,-1, -1, 0, 0,  -1,-1,-1, -1, 0, 0,
        -1,-1,-1, -1, 0, 0,  -1,-1, 1, -1, 0, 0,  -1, 1, 1, -1, 0, 0,
         1, 1, 1,  1, 0, 0,   1, 1,-1,  1, 0, 0,   1,-1,-1,  1, 0, 0,
         1,-1,-1,  1, 0, 0,   1,-1, 1,  1, 0, 0,   1, 1, 1,  1, 0, 0,
        -1,-1,-1,  0,-1, 0,   1,-1,-1,  0,-1, 0,   1,-1, 1,  0,-1, 0,
         1,-1, 1,  0,-1, 0,  -1,-1, 1,  0,-1, 0,  -1,-1,-1,  0,-1, 0,
        -1, 1,-1,  0, 1, 0,   1, 1,-1,  0, 1, 0,   1, 1, 1,  0, 1, 0,
         1, 1, 1,  0, 1, 0,  -1, 1, 1,  0, 1, 0,  -1, 1,-1,  0, 1, 0,
    };
    // scale to ~unit
    for (int i=0;i<(int)(sizeof(verts)/sizeof(float));i+=6) {
        verts[i]/=1.0f; verts[i+1]/=1.0f; verts[i+2]/=1.0f;
    }
    glGenVertexArrays(1, &obj.VAO);
    glGenBuffers(1, &obj.VBO);
    glBindVertexArray(obj.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    obj.vertexCount = 36;
}

// ─── Procedural pyramid ────────────────────────────────────────────────────────

void generatePyramid(SceneObject& obj) {
    float verts[] = {
        // base (two triangles)
        -1,-1,-1,  0,-1, 0,   1,-1,-1,  0,-1, 0,   1,-1, 1,  0,-1, 0,
         1,-1, 1,  0,-1, 0,  -1,-1, 1,  0,-1, 0,  -1,-1,-1,  0,-1, 0,
        // front face
        -1,-1, 1,  0, 0.7f, 0.7f,  1,-1, 1,  0, 0.7f, 0.7f,  0, 1, 0,  0, 0.7f, 0.7f,
        // back face
         1,-1,-1, 0, 0.7f,-0.7f,  -1,-1,-1, 0, 0.7f,-0.7f,  0, 1, 0,  0, 0.7f,-0.7f,
        // right face
         1,-1, 1, 0.7f, 0.7f, 0,   1,-1,-1, 0.7f, 0.7f, 0,  0, 1, 0, 0.7f, 0.7f, 0,
        // left face
        -1,-1,-1, -0.7f, 0.7f, 0, -1,-1, 1, -0.7f, 0.7f, 0, 0, 1, 0, -0.7f, 0.7f, 0,
    };
    glGenVertexArrays(1, &obj.VAO);
    glGenBuffers(1, &obj.VBO);
    glBindVertexArray(obj.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    obj.vertexCount = (int)(sizeof(verts)/sizeof(float)) / 6;
}

// ─── Procedural sphere (UV) ────────────────────────────────────────────────────

void generateSphere(SceneObject& obj, int stacks=16, int slices=16) {
    std::vector<float> verts;
    for (int i=0;i<stacks;i++) {
        float phi0 = M_PI * ((float)i/stacks - 0.5f);
        float phi1 = M_PI * ((float)(i+1)/stacks - 0.5f);
        for (int j=0;j<slices;j++) {
            float th0 = 2*M_PI*(float)j/slices;
            float th1 = 2*M_PI*(float)(j+1)/slices;
            auto push = [&](float phi, float th) {
                float x = cosf(phi)*cosf(th);
                float y = sinf(phi);
                float z = cosf(phi)*sinf(th);
                verts.insert(verts.end(), {x,y,z, x,y,z});
            };
            push(phi0,th0); push(phi1,th0); push(phi1,th1);
            push(phi1,th1); push(phi0,th1); push(phi0,th0);
        }
    }
    glGenVertexArrays(1, &obj.VAO);
    glGenBuffers(1, &obj.VBO);
    glBindVertexArray(obj.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size()*sizeof(float), verts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    obj.vertexCount = (int)verts.size()/6;
}

// ─── Globals ───────────────────────────────────────────────────────────────────

const int SCR_W = 1200, SCR_H = 700;
int  selectedIdx = 0;
bool wireframeChallenge = false;

std::vector<SceneObject> objects;

// Key state helpers
bool keyPressed[GLFW_KEY_LAST+1] = {};

// ─── Callbacks ────────────────────────────────────────────────────────────────

void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
    if (key < 0 || key > GLFW_KEY_LAST) return;
    if (action == GLFW_PRESS)   keyPressed[key] = true;
    if (action == GLFW_RELEASE) keyPressed[key] = false;

    if (action != GLFW_PRESS) return;

    if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, true);

    // Cycle selection
    if (key == GLFW_KEY_TAB || key == GLFW_KEY_N) {
        selectedIdx = (selectedIdx + 1) % (int)objects.size();
        std::cout << "Selected: " << objects[selectedIdx].name << std::endl;
    }
    // Toggle wireframe challenge
    if (key == GLFW_KEY_F) wireframeChallenge = !wireframeChallenge;

    // Reset selected transform
    if (key == GLFW_KEY_HOME) {
        objects[selectedIdx].position = glm::vec3(0);
        objects[selectedIdx].rotation = glm::vec3(0);
        objects[selectedIdx].scale    = glm::vec3(1);
    }
}

// ─── Shader compile/link ──────────────────────────────────────────────────────

GLuint compileShader(GLenum type, const char* src) {
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    int ok; glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512]; glGetShaderInfoLog(s, 512, nullptr, log);
        std::cerr << "Shader error: " << log << std::endl;
    }
    return s;
}

GLuint buildProgram() {
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSrc);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs); glAttachShader(prog, fs);
    glLinkProgram(prog);
    glDeleteShader(vs); glDeleteShader(fs);
    return prog;
}

// ─── Process continuous keyboard input ────────────────────────────────────────

void processInput(float dt) {
    if (objects.empty()) return;
    SceneObject& obj = objects[selectedIdx];

    const float ROT_SPEED   = 60.0f;  // deg/s
    const float TRANS_SPEED = 1.5f;   // units/s
    const float SCALE_SPEED = 1.0f;   // factor/s

    bool rotMode   = keyPressed[GLFW_KEY_R];
    bool transMode = keyPressed[GLFW_KEY_T];
    bool scaleMode = keyPressed[GLFW_KEY_S];

    // ── Rotation (hold R + axis key) ──
    if (rotMode) {
        if (keyPressed[GLFW_KEY_X])           obj.rotation.x += ROT_SPEED * dt;
        if (keyPressed[GLFW_KEY_Z])           obj.rotation.x -= ROT_SPEED * dt;
        if (keyPressed[GLFW_KEY_Y])           obj.rotation.y += ROT_SPEED * dt;
        if (keyPressed[GLFW_KEY_U])           obj.rotation.y -= ROT_SPEED * dt;
        if (keyPressed[GLFW_KEY_C])           obj.rotation.z += ROT_SPEED * dt;
        if (keyPressed[GLFW_KEY_V])           obj.rotation.z -= ROT_SPEED * dt;
    }

    // ── Translation (hold T + arrow/WASD) ──
    if (transMode) {
        if (keyPressed[GLFW_KEY_RIGHT] || keyPressed[GLFW_KEY_D]) obj.position.x += TRANS_SPEED * dt;
        if (keyPressed[GLFW_KEY_LEFT]  || keyPressed[GLFW_KEY_A]) obj.position.x -= TRANS_SPEED * dt;
        if (keyPressed[GLFW_KEY_UP]    || keyPressed[GLFW_KEY_W]) obj.position.y += TRANS_SPEED * dt;
        if (keyPressed[GLFW_KEY_DOWN]  || keyPressed[GLFW_KEY_S] && !scaleMode)
                                                                    obj.position.y -= TRANS_SPEED * dt;
        if (keyPressed[GLFW_KEY_Q])                                obj.position.z += TRANS_SPEED * dt;
        if (keyPressed[GLFW_KEY_E])                                obj.position.z -= TRANS_SPEED * dt;
    }

    // ── Scale (hold S + +/-) ──
    if (scaleMode && !transMode) {
        if (keyPressed[GLFW_KEY_EQUAL] || keyPressed[GLFW_KEY_KP_ADD])
            obj.scale += SCALE_SPEED * dt;
        if (keyPressed[GLFW_KEY_MINUS] || keyPressed[GLFW_KEY_KP_SUBTRACT])
            obj.scale -= SCALE_SPEED * dt;
        // Per-axis
        if (keyPressed[GLFW_KEY_X]) obj.scale.x += SCALE_SPEED * dt;
        if (keyPressed[GLFW_KEY_Z]) obj.scale.x -= SCALE_SPEED * dt;
        if (keyPressed[GLFW_KEY_Y]) obj.scale.y += SCALE_SPEED * dt;
        if (keyPressed[GLFW_KEY_U]) obj.scale.y -= SCALE_SPEED * dt;
        if (keyPressed[GLFW_KEY_C]) obj.scale.z += SCALE_SPEED * dt;
        if (keyPressed[GLFW_KEY_V]) obj.scale.z -= SCALE_SPEED * dt;
        // clamp scale
        obj.scale = glm::max(obj.scale, glm::vec3(0.05f));
    }
}

// ─── Draw HUD text (simple overlay) ──────────────────────────────────────────
// We'll skip FreeType dependency and print to console for clarity.
// A small on-screen quad border around selected object serves as UI.

// ─── Main ─────────────────────────────────────────────────────────────────────

int main(int argc, char** argv) {
    if (!glfwInit()) { std::cerr << "GLFW init failed\n"; return -1; }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_W, SCR_H, "CG - 3D Object Transformations", nullptr, nullptr);
    if (!window) { std::cerr << "Window creation failed\n"; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) { std::cerr << "GLEW init failed\n"; return -1; }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);

    GLuint shader = buildProgram();

    // ── Create / load objects ──
    // Colors
    std::vector<glm::vec3> colors = {
        {0.9f, 0.3f, 0.3f},
        {0.3f, 0.8f, 0.9f},
        {0.9f, 0.85f, 0.2f},
        {0.4f, 0.9f, 0.4f},
    };

    // Try to load .obj files from command line arguments or current directory
    // Otherwise fall back to procedural shapes
    std::vector<std::string> objFiles;
    for (int i = 1; i < argc; i++) objFiles.push_back(argv[i]);

    // Default obj paths to try
    std::vector<std::string> defaultObjs = {"suzanne.obj","monkey.obj","bunny.obj","teapot.obj","cube.obj"};

    if (objFiles.empty()) {
        for (auto& f : defaultObjs) {
            std::ifstream test(f);
            if (test.good()) objFiles.push_back(f);
        }
    }

    // Build 4 objects
    struct ShapeDef { std::string name; int type; std::string file; };
    std::vector<ShapeDef> shapes;
    int objIdx = 0;
    while ((int)shapes.size() < 4) {
        if (objIdx < (int)objFiles.size())
            shapes.push_back({objFiles[objIdx], 0, objFiles[objIdx++]});
        else {
            int t = (int)shapes.size() % 4;
            if      (t==0) shapes.push_back({"Cube",    1, ""});
            else if (t==1) shapes.push_back({"Sphere",  2, ""});
            else if (t==2) shapes.push_back({"Pyramid", 3, ""});
            else           shapes.push_back({"Cube2",   1, ""});
        }
    }

    // Grid layout: 2x2
    std::vector<glm::vec3> positions = {
        {-2.5f,  1.5f, 0.0f},
        { 2.5f,  1.5f, 0.0f},
        {-2.5f, -1.5f, 0.0f},
        { 2.5f, -1.5f, 0.0f},
    };

    for (int i = 0; i < 4; i++) {
        SceneObject obj;
        obj.color    = colors[i % colors.size()];
        obj.name     = shapes[i].name;
        obj.position = positions[i];

        bool loaded = false;
        if (shapes[i].type == 0 && !shapes[i].file.empty())
            loaded = loadOBJ(shapes[i].file, obj);

        if (!loaded) {
            if      (i % 4 == 0) generateCube(obj);
            else if (i % 4 == 1) generateSphere(obj);
            else if (i % 4 == 2) generatePyramid(obj);
            else                 generateCube(obj);
        }

        objects.push_back(obj);
    }

    // Print controls
    std::cout << "\n=== 3D Object Transformation Controls ===\n";
    std::cout << "TAB / N      : cycle selection\n";
    std::cout << "R + X/Z      : rotate X axis (+/-)\n";
    std::cout << "R + Y/U      : rotate Y axis (+/-)\n";
    std::cout << "R + C/V      : rotate Z axis (+/-)\n";
    std::cout << "T + Arrows/WASD : translate XY; Q/E = Z\n";
    std::cout << "S + +/-      : uniform scale\n";
    std::cout << "S + X/Z/Y/U/C/V : per-axis scale\n";
    std::cout << "F            : toggle wireframe overlay\n";
    std::cout << "HOME         : reset selected object\n";
    std::cout << "ESC          : quit\n\n";

    // Camera (fixed orthographic-ish perspective)
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_W/SCR_H, 0.1f, 100.0f);
    glm::mat4 view       = glm::lookAt(glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(0,1,0));

    float lastTime = (float)glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        float now = (float)glfwGetTime();
        float dt  = now - lastTime;
        lastTime  = now;

        glfwPollEvents();
        processInput(dt);

        glClearColor(0.08f, 0.08f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);
        glUniformMatrix4fv(glGetUniformLocation(shader,"view"),       1,GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader,"projection"),  1,GL_FALSE,glm::value_ptr(projection));
        glUniform1i(glGetUniformLocation(shader,"wireframe"), 0);

        for (int i = 0; i < (int)objects.size(); i++) {
            auto& obj = objects[i];
            glm::mat4 model = obj.modelMatrix();
            glUniformMatrix4fv(glGetUniformLocation(shader,"model"), 1, GL_FALSE, glm::value_ptr(model));

            // If selected, brighten color
            glm::vec3 col = obj.color;
            if (i == selectedIdx) col = glm::min(col * 1.4f, glm::vec3(1.0f));
            glUniform3fv(glGetUniformLocation(shader,"objectColor"), 1, glm::value_ptr(col));

            glBindVertexArray(obj.VAO);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawArrays(GL_TRIANGLES, 0, obj.vertexCount);

            // ── CHALLENGE: wireframe overlay ──
            if (wireframeChallenge || i == selectedIdx) {
                glUniform1i(glGetUniformLocation(shader,"wireframe"), 1);
                glm::vec3 wCol = (i == selectedIdx) ? glm::vec3(1.0f, 1.0f, 1.0f) : col;
                glUniform3fv(glGetUniformLocation(shader,"objectColor"), 1, glm::value_ptr(wCol));
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glDrawArrays(GL_TRIANGLES, 0, obj.vertexCount);
                glUniform1i(glGetUniformLocation(shader,"wireframe"), 0);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }

        // Update window title with current state
        auto& s = objects[selectedIdx];
        char title[256];
        snprintf(title, sizeof(title),
            "CG | Selected: %s [%d/%d] | pos(%.1f,%.1f,%.1f) rot(%.0f,%.0f,%.0f) scl(%.2f,%.2f,%.2f)",
            s.name.c_str(), selectedIdx+1, (int)objects.size(),
            s.position.x, s.position.y, s.position.z,
            s.rotation.x, s.rotation.y, s.rotation.z,
            s.scale.x, s.scale.y, s.scale.z);
        glfwSetWindowTitle(window, title);

        glfwSwapBuffers(window);
    }

    for (auto& obj : objects) {
        glDeleteVertexArrays(1, &obj.VAO);
        glDeleteBuffers(1, &obj.VBO);
    }
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
