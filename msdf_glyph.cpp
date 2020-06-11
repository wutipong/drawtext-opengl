#include "msdf_glyph.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

GLuint MsdfGlyph::program;
GLuint MsdfGlyph::vertexShader;
GLuint MsdfGlyph::fragmentShader;
GLuint MsdfGlyph::colorUniform;

const float MsdfGlyph::vertices[] = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

                                     1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

                                     1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

                                     0.0f, 1.0f, 0.0f, 0.0f, 0.0f};

const GLubyte MsdfGlyph::indices[] = {0, 1, 2,

                                      2, 3, 0};

MsdfGlyph::MsdfGlyph(const GLuint &texture, const glm::mat4 &glyphTransform)
    : texture(texture), glyphTransform(glyphTransform) {
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

  glUniform2f(0, context.windowWidth, context.windowHeight);
  glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(glyphTransform));
  glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(transform));

  glBindTexture(GL_TEXTURE_2D, texture);

  glUniform4f(colorUniform, color.r, color.g, color.b, color.a);

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
  colorUniform = glGetUniformLocation(program, "glyphColor");
};

void MsdfGlyph::CleanUp() {
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteProgram(program);
};
