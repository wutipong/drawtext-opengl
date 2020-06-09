#pragma once

#include <SDL2/SDL.h>
#include <gl/glew.h>

#include <memory>
#include <string>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <harfbuzz/hb-ft.h>
#include <harfbuzz/hb.h>

#include <unicode/unistr.h>

#include "glyph.hpp"

class Context;

class Font {
public:
  Font(){};
  Font(const Font &f);
  Font(const Context &context, const std::string filename);

  std::vector<std::shared_ptr<Glyph>>
  CreateGlyphs(const Context &context, const float &start,
               const icu::UnicodeString &str, const int &pixelSize);

  hb_direction_t direction = HB_DIRECTION_LTR;
  hb_script_t script = HB_SCRIPT_LATIN;

private:
  std::shared_ptr<FT_FaceRec> ftFace;
  std::shared_ptr<hb_font_t> hbFont;

  static GLuint CreateTextureFromFT_Bitmap(const FT_Bitmap &bitmap);
};