#include "shader.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <SDL2/SDL.h>

GLuint ReadShader(const GLenum &shaderType, const char *filename) {
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

  GLint result;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

  if (result == GL_FALSE) {
    GLint length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    std::vector<GLchar> log;
    log.reserve(length);

    glGetShaderInfoLog(shader, length, nullptr, log.data());

    SDL_Log("Compile results: %s", log.data());

    throw "error";
  }

  return shader;
}
