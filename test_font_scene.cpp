#include "test_font_scene.hpp"

#include "glyph.hpp"

#include <imgui.h>

void TestFontScene::Tick(Context &context) {
  for (auto g : glyphs) {
    g->Render(context);
  }
}

bool TestFontScene::Init(Context &context) {
  Glyph::Init();
  font = Font(context, "Sarabun-Regular.ttf");
  //glyphs = font.CreateGlyphs(context, "H", 128);
  glyphs = font.CreateGlyphs(context, "A", 64);

  return true;
}
void TestFontScene::Cleanup(Context &context) { Glyph::CleanUp(); }

void TestFontScene::DrawUI(Context &context) {
  ImGui::Begin("Menu");
  context.isDone = ImGui::Button("quit");
  ImGui::End();
}
