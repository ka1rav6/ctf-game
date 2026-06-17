//
// Created by kairav on 6/17/26.
//

#include "Editor.h"
#include "../include/Entity_Components.h"

void Editor::render(Scene& scene) {
    ImGui::Begin("Statistics");
    ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::Begin("Scene Hierarchy");
    auto& reg = scene.getReg();
    auto view = reg.view<IDComponent>();
    for (auto entity : view) {
        std::string label = "Entity " + std::to_string((uint32_t)entity);
        if (reg.all_of<TagComponent>(entity))
            label = reg.get<TagComponent>(entity).name;

        if (ImGui::TreeNode((void*)(intptr_t)entity, "%s", label.c_str())) {
            if (reg.all_of<TransformComponent>(entity)) {
                auto& t = reg.get<TransformComponent>(entity);
                ImGui::DragFloat3("Position", &t.position.x, 0.1f);
                ImGui::DragFloat3("Rotation", &t.rotation.x, 0.1f);
                ImGui::DragFloat3("Scale", &t.scale.x, 0.1f);
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();
}