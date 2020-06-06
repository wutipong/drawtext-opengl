#pragma once

#include "scene.hpp"

#include <memory>
#include <vector>

#include "font.hpp"

class TestFontScene : public Scene {
public:
  virtual bool Init(Context &context);
  virtual void Tick(Context &context);
  virtual void DrawUI(Context &context);
  virtual void Cleanup(Context &context);

private:
  Font font;
  std::vector<std::shared_ptr<Glyph>> glyphs;
};
