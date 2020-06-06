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

    float bearingX = (float) metrics.horiBearingX / 64.0f;
    float bearingY = (float) metrics.horiBearingY / 64.0f;

    x += (float)metrics.horiAdvance / 64.0f;

    auto g = std::make_shared<Glyph>(texture, width, height);
    output.push_back(g);
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

  std::vector<std::uint8_t> buffer;
  buffer.resize(size_t(bitmap.width) * size_t(bitmap.rows));

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
