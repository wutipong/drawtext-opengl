#include "test_font_scene.hpp"

#include "glyph.hpp"

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <unicode/unistr.h>

void TestFontScene::Tick(Context &context) {
  for (auto g : glyphs) {
    g->Render(context, color);
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

  return true;
}
void TestFontScene::Cleanup(Context &context) { Glyph::CleanUp(); }

void TestFontScene::DrawUI(Context &context) {
  std::array<char, 100> buffer;
  std::string str;
  currentText.toUTF8String(str);
  std::fill(buffer.begin(), buffer.end(), 0);
  std::copy(str.begin(), str.end(), buffer.begin());

  ImGui::Begin("Menu");

  ImGui::InputText("Text", buffer.data(), buffer.size());
  ImGui::SliderInt("size", &pixelSize, 0, 128);
  ImGui::ColorEdit4("color", glm::value_ptr(color));
  ImGui::Separator();
  context.isDone = ImGui::Button("quit");

  ImGui::End();

  currentText = icu::UnicodeString::fromUTF8(buffer.data());

  glyphs = fontAtlas.CreateGlyphs(context, currentText, pixelSize);

  for (auto g : glyphs) {
    g->transform = glm::translate(g->transform,
                                  glm::vec3(0, context.windowHeight / 2, 0.0));
  }
}