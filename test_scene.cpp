#include "test_scene.hpp"

#include "imgui_impl_opengl3.h"

#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h> // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h> // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h> // Initialize with gladLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#define GLFW_INCLUDE_NONE // GLFW including OpenGL headers causes ambiguity or
                          // multiple definition errors.
#include <glbinding/Binding.h> // Initialize with glbinding::Binding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#define GLFW_INCLUDE_NONE // GLFW including OpenGL headers causes ambiguity or
                          // multiple definition errors.
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h> // Initialize with glbinding::initialize()
using namespace gl;
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#include <array>
#include <fstream>
#include <imgui.h>
#include <limits>
#include <sstream>
#include <string>

#include "shader.hpp"


template <size_t textureWidth, size_t textureHeight>
static std::array<unsigned char, textureWidth * textureHeight>
GenAlphaTexture() {
  std::array<unsigned char, textureWidth * textureHeight> output;
  for (int i = 0; i < output.size(); i++) {
    auto x = i % textureHeight;
    x *= std::numeric_limits<unsigned char>::max();
    x /= textureWidth;

    output[i] = static_cast<unsigned char>(x);
  }

  return output;
}

bool TestScene::Init(Context &context) {
  auto error = glGetError();

  vertexShader = ReadShader(GL_VERTEX_SHADER, "glyph.vert");
  fragmentShader = ReadShader(GL_FRAGMENT_SHADER, "glyph.frag");
  error = glGetError();

  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  constexpr size_t textureWidth = 64;
  constexpr size_t textureHeight = 64;
  auto data = GenAlphaTexture<textureWidth, textureHeight>();

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, textureWidth, textureHeight, 0, GL_RED,
               GL_UNSIGNED_BYTE, data.data());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glGenVertexArrays(1, &vertexArray);
  glBindVertexArray(vertexArray);
  glGenBuffers(1, &vertexBuffer);

  float vertices[] = {0.0f,
                      0.0f,
                      0.0f,
                      0.0f,
                      1.0f,

                      context.windowWidth,
                      0.0f,
                      0.0f,
                      1.0f,
                      1.0f,

                      context.windowWidth,
                      context.windowHeight,
                      0.0f,
                      1.0f,
                      0.0f,


                      0.0f,
                      context.windowHeight,
                      0.0f,
                      0.0f,
                      0.0f};

  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));

  glGenBuffers(1, &indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

  GLubyte indices[] = {0, 1, 2,

                       2, 3, 0};
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  return true;
};

void TestScene::Cleanup(Context &context) {
  glDeleteBuffers(1, &vertexBuffer);
  glDeleteBuffers(1, &indexBuffer);
  glDeleteVertexArrays(1, &vertexArray);

  glDeleteTextures(1, &texture);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
};

void TestScene::Tick(Context &context) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  auto error = glGetError();

  glBindVertexArray(vertexArray);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glUniform2f(0, context.windowWidth, context.windowHeight);

  glBindTexture(GL_TEXTURE_2D, texture);

  glUseProgram(shaderProgram);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void *)0);

  error = glGetError();

  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
}

void TestScene::DrawUI(Context &context) {
  ImGui::Begin("Menu");
  context.isDone = ImGui::Button("quit");
  ImGui::End();
}
