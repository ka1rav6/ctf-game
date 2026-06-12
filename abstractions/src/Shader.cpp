#include <fstream>
#include <sstream>

#include "../include/shader.h"

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
  // ── 1. Verify context is alive ──────────────────────────────────────────
  const GLubyte *version = glGetString(GL_VERSION);
  if (!version) {
    std::cout << "ERROR::SHADER: No active OpenGL context! "
                 "Shader cannot be created."
              << std::endl;
    ID = 0;
    return;
  }
  std::cout << "OpenGL context OK: " << version << std::endl;

  // ── 2. Read source files ────────────────────────────────────────────────
  std::string vertexCode, fragmentCode;
  std::ifstream vShaderFile, fShaderFile;
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  bool fileReadSuccess = true;
  try {
    std::cout << "Opening: " << vertexPath << std::endl;
    vShaderFile.open(vertexPath);
    std::cout << "Opening: " << fragmentPath << std::endl;
    fShaderFile.open(fragmentPath);

    std::stringstream vss, fss;
    vss << vShaderFile.rdbuf();
    fss << fShaderFile.rdbuf();
    vShaderFile.close();
    fShaderFile.close();

    vertexCode = vss.str();
    fragmentCode = fss.str();
    std::cout << "Shader source read OK (" << vertexCode.size() << " / "
              << fragmentCode.size() << " bytes)" << std::endl;
  } catch (std::ifstream::failure &e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what()
              << std::endl;
    fileReadSuccess = false;
  }

  if (!fileReadSuccess || vertexCode.empty() || fragmentCode.empty()) {
    std::cout << "ERROR::SHADER: Empty source, aborting." << std::endl;
    ID = 0;
    return;
  }

  const char *vCode = vertexCode.c_str();
  const char *fCode = fragmentCode.c_str();

  // ── 3. Compile vertex shader ────────────────────────────────────────────
  unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
  if (!vertex) {
    std::cout << "ERROR::SHADER: glCreateShader(VERTEX) returned 0"
              << std::endl;
    ID = 0;
    return;
  }
  glShaderSource(vertex, 1, &vCode, NULL);
  glCompileShader(vertex);
  checkCompileErrors(vertex, "VERTEX");

  // ── 4. Compile fragment shader ──────────────────────────────────────────
  unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
  if (!fragment) {
    std::cout << "ERROR::SHADER: glCreateShader(FRAGMENT) returned 0"
              << std::endl;
    glDeleteShader(vertex);
    ID = 0;
    return;
  }
  glShaderSource(fragment, 1, &fCode, NULL);
  glCompileShader(fragment);
  checkCompileErrors(fragment, "FRAGMENT");

  // ── 5. Link program ─────────────────────────────────────────────────────
  ID = glCreateProgram();
  if (!ID) {
    std::cout << "ERROR::SHADER: glCreateProgram() returned 0 — "
                 "is there a current OpenGL context?"
              << std::endl;
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return;
  }
  std::cout << "Program ID: " << ID << std::endl;

  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  checkCompileErrors(ID, "PROGRAM");

  glDeleteShader(vertex);
  glDeleteShader(fragment);
  std::cout << "Shader created successfully. ID=" << ID << std::endl;
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
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(mat));
}
void Shader::setVec3(const std::string &name, float v1, float v2,
                     float v3) const {
  glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
}
void Shader::setVec4(const std::string &name, float v1, float v2, float v3,
                     float v4) const {
  glUniform4f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3, v4);
}
void Shader::checkCompileErrors(unsigned int shader, std::string type) {
  int success;
  char infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    // if (!success)
    // {
    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
    std::cout << "ERROR::SHADER_COMPILATION_ERROR [" << type << "]\n"
              << infoLog << std::endl;
    // }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    // if (!success)
    // {
    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
    std::cout << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << std::endl;
    // }
  }
}
