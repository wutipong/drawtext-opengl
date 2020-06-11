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

#include <ext/import-font.h>
#include <msdfgen.h>

#include "msdf_glyph.hpp"

class Context;

class MsdfFont {
public:
  MsdfFont(){};
  MsdfFont(const MsdfFont &f);
  MsdfFont(const Context &context, const std::string filename);

  std::vector<std::shared_ptr<MsdfGlyph>>
  CreateGlyphs(const Context &context, float &x, const icu::UnicodeString &str);

  hb_direction_t direction = HB_DIRECTION_LTR;
  hb_script_t script = HB_SCRIPT_LATIN;

private:
  std::shared_ptr<FT_FaceRec> ftFace;
  std::shared_ptr<hb_font_t> hbMsdfFont;

  static GLuint CreateTexture(const hb_codepoint_t &codepoint,
                              const FT_Face face, int &width, int &height);
};