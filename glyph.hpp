#pragma once

#include <gl/glew.h>

class Glyph {
public:
  Glyph(){};
  Glyph(const Glyph &);
  
  void Render();
  static void Init();
  static void CleanUp();

  friend class Font;

private:
  GLuint texture;
  GLuint vertexBuffer;
  GLuint vertexArray;

  static GLuint program;
  static GLuint vertexShader;
  static GLuint fragmentShader;
};