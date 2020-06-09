#include "font.hpp"
#include "glyph.hpp"

#include <cstddef>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "context.hpp"

Font::Font(const Font &f) : ftFace(f.ftFace), hbFont(f.hbFont) {}

Font::Font(const Context &context, const std::string filename) {
  FT_Face face;
  auto error = FT_New_Face(context.ftLibrary, filename.c_str(), 0, &face);
  if (error) {
    throw "error";
  }
  auto hb_font_ptr = hb_ft_font_create(face, 0);

  ftFace = std::shared_ptr<FT_FaceRec>(face, [](auto f) { FT_Done_Face(f); });
  hbFont = std::shared_ptr<hb_font_t>(hb_font_ptr,
                                      [](auto f) { hb_font_destroy(f); });
}

std::vector<std::shared_ptr<Glyph>>
Font::CreateGlyphs(const Context &context, float &x,
                   const icu::UnicodeString &str, const int &pixelSize) {

  std::vector<std::shared_ptr<Glyph>> output;

  FT_Set_Pixel_Sizes(ftFace.get(), 0, pixelSize);
  hb_ft_font_changed(hbFont.get());

  auto buffer = hb_buffer_create();
  hb_buffer_set_direction(buffer, direction);
  hb_buffer_set_script(buffer, script);
  hb_buffer_add_utf16(buffer,
                      reinterpret_cast<const uint16_t *>(str.getBuffer()),
                      str.length(), 0, str.length());

  hb_shape(hbFont.get(), buffer, NULL, 0);

  auto glyphCount = hb_buffer_get_length(buffer);
  auto glyphInfos = hb_buffer_get_glyph_infos(buffer, NULL);
  auto glyphPositions = hb_buffer_get_glyph_positions(buffer, NULL);

  for (auto i = 0; i < glyphCount; i++) {
    FT_Load_Glyph(ftFace.get(), glyphInfos[i].codepoint, FT_LOAD_RENDER);

    auto bitmap = ftFace->glyph->bitmap;

    auto texture = CreateTextureFromFT_Bitmap(bitmap);
    auto width = bitmap.width;
    auto height = bitmap.rows;

    auto metrics = ftFace->glyph->metrics;

    float bearingX = (float)metrics.horiBearingX / 64.0f;
    float bearingY = (float)metrics.horiBearingY / 64.0f;

    float offsetX = (float)glyphPositions[i].x_offset / 64.0f;
    float offsetY = (float)glyphPositions[i].y_offset / 64.0f;

    glm::mat4 glyphTransform{1.0f};
    glyphTransform = glm::translate(
        glyphTransform,
        glm::vec3(x + bearingX + offsetX, bearingY - height + offsetY, 0.0f));
    glyphTransform = glm::scale(glyphTransform, glm::vec3(width, height, 1.0));

    auto g = new Glyph(texture, glyphTransform);

    output.push_back(std::shared_ptr<Glyph>(g));

    x += (float)glyphPositions[i].x_advance / 64.0f;
  }

  hb_buffer_destroy(buffer);

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

  unsigned char *src = bitmap.buffer;
  char *buffer = new char[size_t(width) * size_t(height)];

  for (int i = 0; i < height; i++) {
    memcpy(buffer + (i * width), src + (i * pitch), width);
  }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED,
               GL_UNSIGNED_BYTE, buffer);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

  delete[] buffer;

  return texture;
}