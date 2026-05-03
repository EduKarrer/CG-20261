#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader {
public:
    GLuint ID;

    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void use() const;
    void setBool (const std::string& name, bool  value) const;
    void setInt  (const std::string& name, int   value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3 (const std::string& name, const glm::vec3& v) const;
    void setVec4 (const std::string& name, const glm::vec4& v) const;
    void setMat4 (const std::string& name, const glm::mat4& m) const;

private:
    std::string readFile(const char* path);
    GLuint compileShader(GLenum type, const std::string& src);
    void checkErrors(GLuint id, const std::string& type);
};
