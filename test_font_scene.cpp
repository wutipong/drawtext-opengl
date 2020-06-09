#include "test_font_scene.hpp"

#include "glyph.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

#include <unicode/unistr.h>

void TestFontScene::Tick(Context &context) {
  for (auto g : glyphs) {
    g->Render(context, glm::vec4(0, 0, 0.7f, 1.0f));
  }
}

bool TestFontScene::Init(Context &context) {
  Glyph::Init();
  fontAtlas.SetDefault(Font{context, "SourceSansPro-Regular.ttf"});
  fontAtlas.AddFont(Font{context, "Sarabun-Regular.ttf"}, USCRIPT_THAI,
                    HB_SCRIPT_THAI);

  auto font = Font(context, "mplus-1c-regular.ttf");

  fontAtlas.AddFont(font, USCRIPT_HIRAGANA, HB_SCRIPT_HIRAGANA);
  fontAtlas.AddFont(font, USCRIPT_KATAKANA, HB_SCRIPT_KATAKANA);
  fontAtlas.AddFont(font, USCRIPT_HAN, HB_SCRIPT_HAN);

  glyphs =
      fontAtlas.CreateGlyphs(context, UNICODE_STRING_SIMPLE("Fuck Off!!"), 64);

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
