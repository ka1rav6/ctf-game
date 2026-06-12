#include "InfiniteGrid.h"
#include "common.h"

InfiniteGrid::InfiniteGrid() {
  shader = new Shader("../src/components/shaders/InfiniteGrid.vs",
                      "../src/components/shaders/InfiniteGrid.fs");
  LOG_INFO("Shader for infinite grid created");
  glGenVertexArrays(1, &VAO);
}

InfiniteGrid::~InfiniteGrid() {
  delete shader;
  shader = nullptr;
  glDeleteVertexArrays(1, &VAO);
}

void InfiniteGrid::render(Camera &camera, const glm::mat4 &projection,
                          float nearPlane, float farPlane) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // LOG_INFO("Infinite Grid rendering now");
  shader->use();
  shader->setVec3("cameraLoc", camera.Position.x, camera.Position.y,
                  camera.Position.z);
  shader->setMat4("view", camera.GetViewMatrix());
  shader->setMat4("projection", projection);
  shader->setFloat("nearPlane", nearPlane);
  shader->setFloat("farPlane", farPlane);
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);

  glDisable(GL_BLEND);
}
