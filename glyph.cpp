#include "glyph.hpp"

#include "shader.hpp"

GLuint Glyph::program;
GLuint Glyph::vertexShader;
GLuint Glyph::fragmentShader;

const float Glyph::vertices[] = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

                                 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

                                 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

                                 0.0f, 1.0f, 0.0f, 0.0f, 0.0f};

const GLubyte Glyph::indices[] = {0, 1, 2,

                                  2, 3, 0};

Glyph::Glyph(const GLuint &texture, const int &width, const int &height)
    : texture(texture), width(width), height(height) {
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

Glyph::~Glyph() {
  if (texture == 0) {
    return;
  }
  glDeleteVertexArrays(1, &vertexArray);
  glDeleteBuffers(bufferSize, buffers);
  glDeleteTextures(1, &texture);
}

void Glyph::Render(const Context &context) {
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

  glVertexAttrib3f(2, width, height, 1.0f);

  glUniform2f(0, context.windowWidth, context.windowHeight);

  glBindTexture(GL_TEXTURE_2D, texture);

  glUseProgram(program);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void *)0);

  error = glGetError();

  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
};
void Glyph::Init() {
  vertexShader = ReadShader(GL_VERTEX_SHADER, "glyph.vert");
  fragmentShader = ReadShader(GL_FRAGMENT_SHADER, "glyph.frag");

  program = glCreateProgram();

  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);
};
void Glyph::CleanUp() {
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteProgram(program);
};
