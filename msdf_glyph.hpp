#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>

#include "context.hpp"
class MsdfGlyph {
public:
  virtual ~MsdfGlyph();

  void Render(const Context &context, const glm::vec4 &color);

  glm::mat4 transform = glm::mat4(1.0f);

  static void Init();
  static void CleanUp();

private:
  friend class MsdfFont;
  MsdfGlyph(){};
  MsdfGlyph(const MsdfGlyph &){};
  MsdfGlyph(const GLuint &texture, const glm::mat4 &glyphTransform);

  static constexpr size_t vertexBufferIndex = 0;
  static constexpr size_t indexBufferIndex = 1;
  static constexpr size_t bufferSize = 2;

  GLuint texture = 0;
  GLuint buffers[bufferSize] = {0, 0};
  GLuint vertexArray = 0;

  const glm::mat4 glyphTransform{1.0f};

  static GLuint program;
  static GLuint vertexShader;
  static GLuint fragmentShader;
  static GLuint colorUniform;
  static GLuint pxRangeUniform;

  static const float vertices[];
  static const GLubyte indices[];
};
