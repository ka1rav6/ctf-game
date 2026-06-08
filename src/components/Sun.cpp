//
// Created by kairav on 6/7/26.
//

#include "Sun.h"

Sun::Sun(unsigned int cubeVAO, const SunSettings& settings)
    : settings(settings),
      shader(settings.vShaderFile.c_str(),settings.fShaderFile.c_str()),
      position(glm::vec3(0.0f)),
      model(glm::mat4(1.0f)),
      cubeVAO(cubeVAO)
{
    LOG_INFO("Sun constructor called");
}

void Sun::update(Camera& camera)
{
    position = camera.Position + settings.sunOffset;

    model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(settings.scale));
}

void Sun::render(Camera& camera, const glm::mat4& projection)
{
    shader.use();

    shader.setMat4("projection", projection);
    shader.setMat4("view", camera.GetViewMatrix());
    shader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}