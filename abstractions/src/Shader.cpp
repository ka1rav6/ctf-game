#include <fstream>
#include <sstream>

#include "../include/shader.h"

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
  std::string vertexCode, fragmentCode;
  std::ifstream vShaderFile, fShaderFile;
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  bool fileReadSuccess = true;
  try {
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);

    std::stringstream vss, fss;
    vss << vShaderFile.rdbuf();
    fss << fShaderFile.rdbuf();
    vShaderFile.close();
    fShaderFile.close();

    vertexCode = vss.str();
    fragmentCode = fss.str();
  } catch (std::ifstream::failure &e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    fileReadSuccess = false;
  }

  if (!fileReadSuccess || vertexCode.empty() || fragmentCode.empty()) {
    std::cout << "ERROR::SHADER: Empty source, aborting." << std::endl;
    ID = 0;
    return;
  }

  const char *vCode = vertexCode.c_str();
  const char *fCode = fragmentCode.c_str();

  unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
  if (!vertex) {
    std::cout << "ERROR::SHADER: glCreateShader(VERTEX) returned 0" << std::endl;
    ID = 0;
    return;
  }
  glShaderSource(vertex, 1, &vCode, NULL);
  glCompileShader(vertex);
  checkCompileErrors(vertex, "VERTEX");

  unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
  if (!fragment) {
    std::cout << "ERROR::SHADER: glCreateShader(FRAGMENT) returned 0" << std::endl;
    glDeleteShader(vertex);
    ID = 0;
    return;
  }
  glShaderSource(fragment, 1, &fCode, NULL);
  glCompileShader(fragment);
  checkCompileErrors(fragment, "FRAGMENT");

  ID = glCreateProgram();
  if (!ID) {
    std::cout << "ERROR::SHADER: glCreateProgram() returned 0" << std::endl;
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return;
  }

  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  checkCompileErrors(ID, "PROGRAM");

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::use() const { glUseProgram(ID); }

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::setVec3(const std::string &name, float v1, float v2, float v3) const {
  glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
}
void Shader::setVec4(const std::string &name, float v1, float v2, float v3, float v4) const {
  glUniform4f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3, v4);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type) {
  int success;
  char infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "ERROR::SHADER_COMPILATION_ERROR [" << type << "]\n" << infoLog << std::endl;
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << std::endl;
    }
  }
}