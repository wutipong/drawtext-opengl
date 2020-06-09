#pragma once

#include <SDL2/SDL.h>
#include <gl/glew.h>

#include <memory>
#include <string>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "glyph.hpp"

class Context;

class Font {
public:
  Font(){};
  Font(const Font &f);
  Font(const Context &context, const std::string filename);

  std::vector<std::shared_ptr<Glyph>> CreateGlyphs(const Context &context,
                                                   const float &start,
                                                   const std::string &text,
                                                   const int &pixelSize);

private:
  std::shared_ptr<FT_FaceRec> ftFace;
  static GLuint CreateTextureFromFT_Bitmap(const FT_Bitmap &bitmap);
};