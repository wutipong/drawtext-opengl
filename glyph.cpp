#include "glyph.hpp"

#include "shader.hpp"

GLuint Glyph::program;
GLuint Glyph::vertexShader;
GLuint Glyph::fragmentShader;

Glyph::Glyph(const Glyph &g)
    : texture(g.texture), vertexBuffer(g.vertexBuffer),
      vertexArray(g.vertexArray) {}

void Glyph::Render(){};
void Glyph::Init(){
  vertexShader = ReadShader(GL_VERTEX_SHADER, "glyph.vert");
  fragmentShader = ReadShader(GL_FRAGMENT_SHADER, "glyph.frag");
};
void Glyph::CleanUp(){
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteProgram(program);
};
