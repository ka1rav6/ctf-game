//
// Created by kairav on 6/9/26.
//

#include "Renderer.h"
#include "../include/Entity_Components.h"
#include "../src/components/InfiniteGrid.h"
#include "../src/components/Sun.h"

void Renderer::render(Scene &scene, Camera &camera,
                      const glm::mat4 &projection) {
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LESS);
  auto &registry = scene.getReg();

  auto sunView = registry.view<SunComponent>();
  for (auto entity : sunView) {
    auto &sc = sunView.get<SunComponent>(entity);
    if (sc.sun) {
      sc.sun->render(camera, projection);
    }
  }

  glDisable(GL_BLEND);
  glDisable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  auto view = registry.view<TransformComponent, MeshRendererComponent>();

  for (auto entity : view) {
    // Destructure: grab references to the components we need
    auto &transform = view.get<TransformComponent>(entity);
    auto &meshRenderer = view.get<MeshRendererComponent>(entity);

    // Safety check — skip if model or shader hasn't been set
    if (!meshRenderer.model || !meshRenderer.shader)
      continue;

    // Activate this entity's shader program
    meshRenderer.shader->use();
    glm::mat4 model(1.0f);
    model = glm::translate(model, transform.position);
    model *= glm::mat4_cast(transform.orientation);
    model = glm::scale(model, transform.scale);

    meshRenderer.shader->setMat4("model", model);
    meshRenderer.shader->setMat4("view", camera.GetViewMatrix());
    meshRenderer.shader->setMat4("projection", projection);

    // Draw all the meshes in the model
    meshRenderer.model->Draw(*meshRenderer.shader);
  }

  auto gridView = registry.view<InfiniteGridComponent>();
  for (auto entity : gridView) {
    auto &gc = gridView.get<InfiniteGridComponent>(entity);
    if (gc.grid) {
      gc.grid->render(camera, projection, 0.1f, 1000.0f);
    }
  }
}