#pragma once

#include <gl/glew.h>

#include "context.hpp"
class Glyph {
public:

  virtual ~Glyph();

  void Render(const Context& context);
  static void Init();
  static void CleanUp();



private:
  friend class Font;
  Glyph(){};
  Glyph(const Glyph &){};
  Glyph(const GLuint &texture, const int &width, const int &height);

  static constexpr size_t vertexBufferIndex = 0;
  static constexpr size_t indexBufferIndex = 1;
  static constexpr size_t bufferSize = 2;

  GLuint texture;
  GLuint buffers[bufferSize];
  GLuint vertexArray = 0;

  const int width = 0;
  const int height = 0;

  static GLuint program;
  static GLuint vertexShader;
  static GLuint fragmentShader;

  static const float vertices[];
  static const GLubyte indices[];
};