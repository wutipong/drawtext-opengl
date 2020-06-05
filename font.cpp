#include "font.hpp"

#include <cstddef>
#include <utf8.h>

#include "context.hpp"

Font::Font(const Font &f) : ftFace(f.ftFace) {}

Font::Font(const Context &context, const std::string filename) {

  FT_Face face;
  auto error = FT_New_Face(context.ftLibrary, filename.c_str(), 0, &face);
  if (error) {
    throw "error";
  }

  ftFace = std::shared_ptr<FT_FaceRec>(face, [](auto f) { FT_Done_Face(f); });
}

std::vector<Glyph> Font::CreateGlyph(const Context &context,
                                     const std::string &text,
                                     const int &pixelSize) {
  std::vector<Glyph> output;

  std::vector<FT_ULong> charactors;
  auto end_it = utf8::find_invalid(text.begin(), text.end());
  utf8::utf8to16(text.begin(), end_it, std::back_inserter(charactors));

  float x = 0;

  for (auto c : charactors) {
    FT_Load_Char(ftFace.get(), c, FT_LOAD_RENDER);

    auto texture = CreateTextureFromFT_Bitmap(ftFace->glyph->bitmap);
    float bearingX = (float)ftFace->glyph->metrics.horiBearingX / 64.0f;
    float bearingY = (float)ftFace->glyph->metrics.horiBearingY / 64.0f;

    x += (float)(ftFace->glyph->metrics.horiAdvance) / 64.0f;
  }

  return output;
}

GLuint Font::CreateTextureFromFT_Bitmap(const FT_Bitmap &bitmap) {
  GLuint texture;
  glGenTextures(1, &texture);

  if (bitmap.width == 0 || bitmap.rows == 0) {
    return texture;
  }

  glBindTexture(GL_TEXTURE_2D, texture);

  std::vector<std::uint8_t> buffer(bitmap.width * bitmap.rows);

  auto width = bitmap.width;
  auto height = bitmap.rows;
  auto pitch = bitmap.pitch;

  unsigned char *src_pixels = bitmap.buffer;

  for (unsigned int y = 0; y < bitmap.rows; y++) {
    memcpy(buffer.data() + y * width, bitmap.buffer + y * pitch, width);
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED,
               GL_UNSIGNED_BYTE, buffer.data());

  return texture;
}
