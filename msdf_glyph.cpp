#include "msdf_glyph.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

GLuint MsdfGlyph::program;
GLuint MsdfGlyph::vertexShader;
GLuint MsdfGlyph::fragmentShader;
GLuint MsdfGlyph::colorUniform;
GLuint MsdfGlyph::pxRangeUniform;

const float MsdfGlyph::vertices[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                                     1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

                                     1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

                                     0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

const GLubyte MsdfGlyph::indices[] = {0, 1, 2,

                                      2, 3, 0};

MsdfGlyph::MsdfGlyph(const GLuint &texture, const glm::vec2 &glyphPos,
                     const glm::vec2 &glyphSize,
                     const float &actualPixelSize)
    : texture(texture), glyphPos(glyphPos),
      glyphSize(glyphSize),
      actualPixelSize(actualPixelSize) {
  if (texture == 0) {
    return;
  }

  glGenVertexArrays(1, &vertexArray);
  glBindVertexArray(vertexArray);

  glGenBuffers(bufferSize, buffers);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[vertexBufferIndex]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[indexBufferIndex]);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[indexBufferIndex]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
}

MsdfGlyph::~MsdfGlyph() {
  if (texture == 0) {
    return;
  }
  glDeleteVertexArrays(1, &vertexArray);
  glDeleteBuffers(bufferSize, buffers);
  glDeleteTextures(1, &texture);
}

void MsdfGlyph::Render(const Context &context, const glm::vec4 &color) {
  // Texture #0 means no texture associated to the glyph.
  // There's nothing to draw.
  if (texture == 0)
    return;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  auto error = glGetError();

  glBindVertexArray(vertexArray);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[vertexBufferIndex]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[indexBufferIndex]);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  float scale = pixelSize / actualPixelSize;

  glUniform2f(0, context.windowWidth, context.windowHeight);
  glUniform2f(1, glyphSize.x, glyphSize.y);
  glUniform2f(2, glyphPos.x, glyphPos.y);
  glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(transform));
  glUniform1f(4, scale);

  glBindTexture(GL_TEXTURE_2D, texture);

  constexpr float pxRange = 2.0f;

  glUniform4f(colorUniform, color.r, color.g, color.b, color.a);
  glUniform1f(pxRangeUniform, pxRange);

  glUseProgram(program);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void *)0);

  error = glGetError();

  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
};

void MsdfGlyph::Init() {
  vertexShader = ReadShader(GL_VERTEX_SHADER, "msdf.vert");
  fragmentShader = ReadShader(GL_FRAGMENT_SHADER, "msdf.frag");

  program = glCreateProgram();

  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  glUseProgram(program);
  colorUniform = glGetUniformLocation(program, "fgColor");
  pxRangeUniform = glGetUniformLocation(program, "pxRange");
};

void MsdfGlyph::CleanUp() {
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteProgram(program);
};
