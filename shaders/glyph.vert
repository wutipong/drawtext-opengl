#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

layout(location = 0) uniform vec2 screen;

out vec2 TexCoord;

void main() {
  gl_Position = vec4(2 * (aPos.x / screen.x) - 1.0f, 2 * (aPos.y / screen.y) - 1.0f,
                     aPos.z, 1.0);

  TexCoord = aTexCoord;
}
