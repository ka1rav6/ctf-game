//
// Created by kairav on 6/9/26.
//

#include "Renderer.h"
#include "../include/Entity_Components.h"
void Renderer::render(Scene& scene, Camera& camera)
{
    auto& registry = scene.getReg();

    auto view = registry.view< Transform, MeshRenderer>();
    for(auto entity : view) {
        auto& transform = view.get<Transform>(entity);
        auto& renderer = view.get<MeshRenderer>(entity);

        if(!renderer.model || !renderer.shader)
            continue;

        renderer.shader->use();

        glm::mat4 model(1.0f);

        model = glm::translate(model, transform.position);

        model = glm::rotate(model,glm::radians(transform.rotation.x),glm::vec3(1,0,0));
        model = glm::rotate(model,glm::radians(transform.rotation.y),glm::vec3(0,1,0));
        model = glm::rotate(model,glm::radians(transform.rotation.z),glm::vec3(0,0,1));
        model = glm::scale(model,transform.scale);
        renderer.shader->setMat4("model",model);
        renderer.shader->setMat4("view",camera.GetViewMatrix());
        renderer.shader->setMat4("projection",camera.getProjectionMatrix());
        renderer.model->Draw(*renderer.shader);
    }
}