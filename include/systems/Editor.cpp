//
// Created by kairav on 6/17/26.
//

#include "Editor.h"
#include "../include/Entity_Components.h"

void Editor::render(Scene& scene, bool editorMode) {
    ImGui::Begin("Statistics");
    ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
    ImGui::Text("Press F1 to toggle Editor Mode");
    ImGui::End();

    if (editorMode) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->WorkSize.x * 0.5f, viewport->WorkPos.y + 10), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
        ImGui::SetNextWindowBgAlpha(0.65f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(24.0f, 14.0f));
        ImGui::Begin("##EditorModeBanner", nullptr,
            ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
        ImGui::SetWindowFontScale(2.0f);
        ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.2f, 1.0f), "EDITOR MODE  (F1 to exit)");
        ImGui::End();
        ImGui::PopStyleVar();
    }

    if (!editorMode) return;

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
                bool posChanged = ImGui::DragFloat3("Position", &t.position.x, 0.1f);
                glm::vec3 euler = glm::degrees(glm::eulerAngles(t.orientation));
                bool rotChanged = ImGui::DragFloat3("Rotation", &euler.x, 0.1f);
                if (rotChanged) {
                    t.orientation = glm::quat(glm::radians(euler));
                }
                bool scaleChanged = ImGui::DragFloat3("Scale", &t.scale.x, 0.1f);

                if ((posChanged || rotChanged || scaleChanged) && reg.all_of<RigidBodyComponent>(entity)) {
                    auto& rb = reg.get<RigidBodyComponent>(entity);
                    if (rb.body) {
                        using namespace reactphysics3d;
                        rb.body->setTransform(Transform(
                            Vector3(t.position.x, t.position.y, t.position.z),
                            Quaternion(t.orientation.x, t.orientation.y,
                                       t.orientation.z, t.orientation.w)));
                    }
                }
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();
}