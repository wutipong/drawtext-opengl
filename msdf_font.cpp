#include "msdf_font.hpp"
#include "glyph.hpp"

#include <cstddef>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "context.hpp"

MsdfFont::MsdfFont(const MsdfFont &f)
    : ftFace(f.ftFace), hbMsdfFont(f.hbMsdfFont) {}

MsdfFont::MsdfFont(const Context &context, const std::string filename) {
  FT_Face face;
  auto error = FT_New_Face(context.ftLibrary, filename.c_str(), 0, &face);
  if (error) {
    throw "error";
  }
  auto hb_font_ptr = hb_ft_font_create(face, 0);

  ftFace = std::shared_ptr<FT_FaceRec>(face, [](auto f) { FT_Done_Face(f); });
  hbMsdfFont = std::shared_ptr<hb_font_t>(hb_font_ptr,
                                          [](auto f) { hb_font_destroy(f); });
}

std::vector<std::shared_ptr<MsdfGlyph>>
MsdfFont::CreateGlyphs(const Context &context, float &x,
                       const icu::UnicodeString &str) {

  std::vector<std::shared_ptr<MsdfGlyph>> output;

  FT_Size_RequestRec req;

  req.type = FT_SIZE_REQUEST_TYPE_NOMINAL;
  req.width = ftFace->units_per_EM;
  req.height = ftFace->units_per_EM;
  req.horiResolution = 0;
  req.vertResolution = 0;

  FT_Request_Size(ftFace.get(), &req);

  hb_ft_font_changed(hbMsdfFont.get());

  auto buffer = hb_buffer_create();
  hb_buffer_set_direction(buffer, direction);
  hb_buffer_set_script(buffer, script);
  hb_buffer_add_utf16(buffer,
                      reinterpret_cast<const uint16_t *>(str.getBuffer()),
                      str.length(), 0, str.length());

  hb_shape(hbMsdfFont.get(), buffer, NULL, 0);

  auto glyphCount = hb_buffer_get_length(buffer);
  auto glyphInfos = hb_buffer_get_glyph_infos(buffer, NULL);
  auto glyphPositions = hb_buffer_get_glyph_positions(buffer, NULL);

  for (auto i = 0; i < glyphCount; i++) {

    auto bitmap = ftFace->glyph->bitmap;

    int width, height;
    auto texture =
        CreateTexture(glyphInfos[i].codepoint, ftFace.get(), width, height);

    auto metrics = ftFace->glyph->metrics;

    float bearingX = (float)metrics.horiBearingX / 64.0f;
    float bearingY = (float)metrics.horiBearingY / 64.0f;

    float offsetX = (float)glyphPositions[i].x_offset / 64.0f;
    float offsetY = (float)glyphPositions[i].y_offset / 64.0f;

    glm::mat4 glyphTransform{1.0f};
    glyphTransform = glm::translate(
        glyphTransform, glm::vec3(x + bearingX + offsetX - 4.0f,
                                  bearingY + offsetY - height - 4.0f, 0.0f));
    glyphTransform = glm::scale(glyphTransform, glm::vec3(width, height, 1.0));

    auto g = new MsdfGlyph(texture, glyphTransform,
                           1 / 64.0f * ftFace->units_per_EM);

    output.push_back(std::shared_ptr<MsdfGlyph>(g));

    x += (float)glyphPositions[i].x_advance / 64.0f;
  }

  hb_buffer_destroy(buffer);

  return output;
}

GLuint MsdfFont::CreateTexture(const hb_codepoint_t &codepoint, FT_Face face,
                               int &width, int &height) {
  GLuint texture;
  glGenTextures(1, &texture);

  msdfgen::Shape shape;

  msdfgen::loadGlyphIndex(shape, face, codepoint);
  shape.normalize();
  msdfgen::edgeColoringSimple(shape, 3, 0);

  auto bound = shape.getBounds();
  width = 8 + bound.r - bound.l;
  height = 8 + bound.t - bound.b;

  msdfgen::Bitmap<float, 3> bitmap(width, height);

  constexpr double pxRange = 2.0;
  msdfgen::generateMSDF(bitmap, shape, pxRange, msdfgen::Vector2(1.0, 1.0),
                        msdfgen::Vector2(-bound.l + 4, -bound.b + 4));

  glBindTexture(GL_TEXTURE_2D, texture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT,
               bitmap.operator const float *());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

  return texture;
}