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

#include <fstream>
#include <imgui.h>
#include <sstream>
#include <string>

static GLuint ReadShader(const GLenum &shaderType, const char *filename) {
  auto shader = glCreateShader(shaderType);

  std::fstream fs(filename, std::ios_base::in);
  std::stringstream buffer;
  buffer << fs.rdbuf();

  fs.close();

  auto src = buffer.str();
  GLchar const *files[] = {src.c_str()};
  GLint lengths[] = {src.size()};

  glShaderSource(shader, 1, files, lengths);
  glCompileShader(shader);

  return shader;
}

bool TestScene::Init(Context &context) {
  
  vertexShader = ReadShader(GL_VERTEX_SHADER, "glyph.vert");
  fragmentShader = ReadShader(GL_FRAGMENT_SHADER, "glyph.frag");
  
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  return true;
};

void TestScene::Cleanup(Context &context) {
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
};

void TestScene::Tick(Context &context) {
  glUseProgram(shaderProgram);

  unsigned int VBO;
  glGenBuffers(1, &VBO);

  float vertices[] = {
      0.0f, 0.0f, 0.0f, 
      context.windowWidth, 0.0f, 0.0f, 
      context.windowWidth, context.windowHeight, 0.0f, 

      context.windowWidth, context.windowHeight, 0.0f,
      0.0f, context.windowHeight, 0.0f,
      0.0f, 0.0f, 0.0f
  };

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttrib1f(1, context.windowWidth);
  glVertexAttrib1f(2, context.windowHeight);

  glUseProgram(shaderProgram);

  glDrawArrays(GL_TRIANGLES, 0, 6);

  glDeleteBuffers(1, &VBO);
}

void TestScene::DrawUI(Context &context) {
  ImGui::Begin("Menu");
  context.isDone = ImGui::Button("quit");
  ImGui::End();
}
