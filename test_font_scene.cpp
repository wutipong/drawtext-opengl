#include "test_font_scene.hpp"

#include "glyph.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

void TestFontScene::Tick(Context &context) {
  for (auto g : glyphs) {
    g->Render(context, glm::vec4(0, 0, 0.7f, 1.0f));
  }
}

bool TestFontScene::Init(Context &context) {
  Glyph::Init();
  font = Font(context, "Sarabun-Regular.ttf");
  glyphs = font.CreateGlyphs(context, 0, "Fuck Off!!", 64);

  for (auto g : glyphs) {
    g->transform =
        glm::translate(g->transform, glm::vec3(context.windowWidth / 2,
                                               context.windowHeight / 2, 0.0));
  }

  return true;
}
void TestFontScene::Cleanup(Context &context) { Glyph::CleanUp(); }

void TestFontScene::DrawUI(Context &context) {
  ImGui::Begin("Menu");
  context.isDone = ImGui::Button("quit");
  ImGui::End();
}
