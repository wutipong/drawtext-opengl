#pragma once

#include "scene.hpp"

#include <algorithm>
#include <array>
#include <memory>
#include <vector>

#include "msdf_font.hpp"
#include "msdf_font_atlas.hpp"

class TestMsdfFontScene : public Scene {
public:
  virtual bool Init(Context &context);
  virtual void Tick(Context &context);
  virtual void DrawUI(Context &context);
  virtual void Cleanup(Context &context);

private:
  MsdfFontAtlas fontAtlas;
  std::vector<std::shared_ptr<MsdfGlyph>> glyphs;
  std::array<char, 100> buffer;
  int pixelSize = 64;
  glm::vec4 color = {0, 0, 0, 1.0f};
};
