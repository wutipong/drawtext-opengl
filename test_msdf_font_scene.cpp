#include "test_msdf_font_scene.hpp"

#include "msdf_glyph.hpp"
#include "test_font_scene.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <unicode/unistr.h>

void TestMsdfFontScene::Tick(Context &context) {
  for (auto g : glyphs) {
    g->Render(context, color);
  }
}

bool TestMsdfFontScene::Init(Context &context) {
  MsdfGlyph::Init();
  fontAtlas.SetDefault(MsdfFont{context, "SourceSansPro-Regular.ttf"});
  fontAtlas.AddMsdfFont(MsdfFont{context, "Sarabun-Regular.ttf"}, USCRIPT_THAI,
                        HB_SCRIPT_THAI);

  auto font = MsdfFont(context, "mplus-1c-regular.ttf");

  fontAtlas.AddMsdfFont(font, USCRIPT_HIRAGANA, HB_SCRIPT_HIRAGANA);
  fontAtlas.AddMsdfFont(font, USCRIPT_KATAKANA, HB_SCRIPT_KATAKANA);
  fontAtlas.AddMsdfFont(font, USCRIPT_HAN, HB_SCRIPT_HAN);

  std::fill(buffer.begin(), buffer.end(), 0);

  return true;
}
void TestMsdfFontScene::Cleanup(Context &context) { MsdfGlyph::CleanUp(); }

void TestMsdfFontScene::DrawUI(Context &context) {

  ImGui::Begin("Multi-channel SDF");

  bool textChanged = ImGui::InputText("Text", buffer.data(), buffer.size());
  bool sizeChanged = ImGui::SliderInt("size", &pixelSize, 0, 128);
  bool colorChanged = ImGui::ColorEdit4("color", glm::value_ptr(color));
  ImGui::Separator();
  bool sceneChange = ImGui::Button("Freetype Bitmap");
  context.isDone = ImGui::Button("quit");

  ImGui::End();

  if (sceneChange) {
    Scene::ChangeScene<TestFontScene>(context);
  }

  if (textChanged || sizeChanged || colorChanged) {
    auto text = icu::UnicodeString::fromUTF8(buffer.data());

    glyphs = fontAtlas.CreateGlyphs(context, text, pixelSize);

    for (auto g : glyphs) {
      g->transform = glm::translate(
          g->transform, glm::vec3(0, context.windowHeight / 2, 0.0));
    }
  }
}