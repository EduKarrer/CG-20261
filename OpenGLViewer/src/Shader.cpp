#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vSrc = readFile(vertexPath);
    std::string fSrc = readFile(fragmentPath);

    GLuint vert = compileShader(GL_VERTEX_SHADER,   vSrc);
    GLuint frag = compileShader(GL_FRAGMENT_SHADER, fSrc);

    ID = glCreateProgram();
    glAttachShader(ID, vert);
    glAttachShader(ID, frag);
    glLinkProgram(ID);
    checkErrors(ID, "PROGRAM");

    glDeleteShader(vert);
    glDeleteShader(frag);
}

Shader::~Shader() { glDeleteProgram(ID); }

void Shader::use() const { glUseProgram(ID); }

void Shader::setBool (const std::string& n, bool  v) const { glUniform1i (glGetUniformLocation(ID, n.c_str()), (int)v); }
void Shader::setInt  (const std::string& n, int   v) const { glUniform1i (glGetUniformLocation(ID, n.c_str()), v); }
void Shader::setFloat(const std::string& n, float v) const { glUniform1f (glGetUniformLocation(ID, n.c_str()), v); }
void Shader::setVec3 (const std::string& n, const glm::vec3& v) const { glUniform3fv(glGetUniformLocation(ID, n.c_str()), 1, glm::value_ptr(v)); }
void Shader::setVec4 (const std::string& n, const glm::vec4& v) const { glUniform4fv(glGetUniformLocation(ID, n.c_str()), 1, glm::value_ptr(v)); }
void Shader::setMat4 (const std::string& n, const glm::mat4& m) const { glUniformMatrix4fv(glGetUniformLocation(ID, n.c_str()), 1, GL_FALSE, glm::value_ptr(m)); }

std::string Shader::readFile(const char* path) {
    std::ifstream f(path);
    if (!f.is_open())
        throw std::runtime_error(std::string("Cannot open shader: ") + path);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

GLuint Shader::compileShader(GLenum type, const std::string& src) {
    GLuint id = glCreateShader(type);
    const char* c = src.c_str();
    glShaderSource(id, 1, &c, nullptr);
    glCompileShader(id);
    checkErrors(id, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
    return id;
}

void Shader::checkErrors(GLuint id, const std::string& type) {
    GLint ok; GLchar log[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
        if (!ok) {
            glGetShaderInfoLog(id, 1024, nullptr, log);
            std::cerr << "[Shader " << type << " ERROR]\n" << log << "\n";
        }
    } else {
        glGetProgramiv(id, GL_LINK_STATUS, &ok);
        if (!ok) {
            glGetProgramInfoLog(id, 1024, nullptr, log);
            std::cerr << "[Shader LINK ERROR]\n" << log << "\n";
        }
    }
}
