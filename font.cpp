#include "font.hpp"
#include "glyph.hpp"

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

std::vector<std::shared_ptr<Glyph>> Font::CreateGlyphs(const Context &context,
                                                       const std::string &text,
                                                       const int &pixelSize) {
  std::vector<std::shared_ptr<Glyph>> output;

  std::vector<FT_ULong> charactors;
  auto end_it = utf8::find_invalid(text.begin(), text.end());
  utf8::utf8to16(text.begin(), end_it, std::back_inserter(charactors));

  float x = 0;

  FT_Set_Pixel_Sizes(ftFace.get(), 0, pixelSize);

  for (auto c : charactors) {
    FT_Load_Char(ftFace.get(), c, FT_LOAD_RENDER);

    auto bitmap = ftFace->glyph->bitmap;

    auto texture = CreateTextureFromFT_Bitmap(bitmap);
    auto width = bitmap.width;
    auto height = bitmap.rows;

    auto metrics = ftFace->glyph->metrics;

    float bearingX = (float)metrics.horiBearingX / 64.0f;
    float bearingY = (float)metrics.horiBearingY / 64.0f;

    x += (float)metrics.horiAdvance / 64.0f;

    auto g = new Glyph(texture, width, height);
    output.push_back(std::shared_ptr<Glyph>(g));
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

  auto width = bitmap.width;
  auto height = bitmap.rows;
  auto pitch = bitmap.pitch;

  unsigned char *src_pixels = bitmap.buffer;
  char *buffer = new char[size_t(width) * size_t(height)];

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      buffer[(y * width) + x] = src_pixels[(y * pitch) + x];
    }
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, pitch, 0, GL_RED,
               GL_UNSIGNED_BYTE, buffer);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

  delete[] buffer;

  return texture;
}
