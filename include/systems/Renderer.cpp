//
// Created by kairav on 6/9/26.
//

#include "Renderer.h"
#include "../include/Entity_Components.h"
#include "../src/components/Sun.h"
#include "../src/components/InfiniteGrid.h"

void Renderer::render(Scene& scene, Camera& camera, PhysicsEngine* e, const glm::mat4& projection) {
    auto& registry = scene.getReg();

    auto sunView = registry.view<SunComponent>();
    for (auto entity : sunView) {
        auto& sc = sunView.get<SunComponent>(entity);
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
            auto& transform = view.get<TransformComponent>(entity);
            auto& meshRenderer = view.get<MeshRendererComponent>(entity);

            // Safety check — skip if model or shader hasn't been set
            if (!meshRenderer.model || !meshRenderer.shader)
                continue;

            // Activate this entity's shader program
            meshRenderer.shader->use();
            glm::mat4 model(1.0f);
            model = glm::translate(model, transform.position);
            model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1, 0, 0));
            model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0, 1, 0));
            model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));
            model = glm::scale(model, transform.scale);

            meshRenderer.shader->setMat4("model", model);
            meshRenderer.shader->setMat4("view", camera.GetViewMatrix());
            meshRenderer.shader->setMat4("projection", projection);

            // Draw all the meshes in the model
            meshRenderer.model->Draw(*meshRenderer.shader);
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);

        auto gridView = registry.view<InfiniteGridComponent>();
        for (auto entity : gridView) {
            auto& gc = gridView.get<InfiniteGridComponent>(entity);
            if (gc.grid) {
                gc.grid->render(camera, projection, 0.1f, 1000.0f);
            }
        }
        glDepthMask(GL_TRUE);

        auto physicsView = registry.view<RigidBodyComponent>();
        for (auto entity : physicsView) {
            auto& rigidBody = physicsView.get<RigidBodyComponent>(entity);
        }

}