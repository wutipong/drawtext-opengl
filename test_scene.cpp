#include "test_scene.hpp"

#include <imgui.h>

void TestScene::Tick(Context &context) {}
void TestScene::DrawUI(Context &context) {
  ImGui::Begin("Menu");
  context.isDone = ImGui::Button("quit");
  ImGui::End();
}