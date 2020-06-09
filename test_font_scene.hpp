#pragma once

#include "scene.hpp"

#include <memory>
#include <vector>

#include "font.hpp"
#include "font_atlas.hpp"

class TestFontScene : public Scene {
public:
  virtual bool Init(Context &context);
  virtual void Tick(Context &context);
  virtual void DrawUI(Context &context);
  virtual void Cleanup(Context &context);

private:
  FontAtlas fontAtlas;
  std::vector<std::shared_ptr<Glyph>> glyphs;
  icu::UnicodeString currentText;
  int pixelSize = 64;
  glm::vec4 color = {0, 0, 0, 1.0f};
};
