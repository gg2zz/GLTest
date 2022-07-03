#include "Shader.h"

namespace ZG {
unsigned int Shader::_LoadShader(GLenum type, const char* path) {
    std::string sShader;
    std::ifstream ShaderFile;
    ShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        ShaderFile.open(SHADER_PATH + path);
        std::stringstream ss;

        // read file buffer
        ss << ShaderFile.rdbuf();

        ShaderFile.close();
        sShader = ss.str();
    } catch (std::ifstream::failure e) {
        std::cout << "LIZHENHHUI" << std::endl;
        std::cout << SHADER_PATH << std::endl;
        std::cout << "ERROR:SHADER:FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    const char* code = sShader.c_str();

    int success;
    char infoLog[512];
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &code, NULL);
    glCompileShader(id);
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0;
    }

    return id;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    auto vid = _LoadShader(GL_VERTEX_SHADER, vertexPath);
    auto fid = _LoadShader(GL_FRAGMENT_SHADER, fragmentPath);
    UID = glCreateProgram();
    glAttachShader(UID, vid);
    glAttachShader(UID, fid);
    glLinkProgram(UID);
    int success;
    char infoLog[512];
    glGetProgramiv(UID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(UID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vid);
    glDeleteShader(fid);
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(UID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(UID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(UID, name.c_str()), value);
}

void Shader::use() {
    glUseProgram(this->UID);
}
}  // namespace ZG
