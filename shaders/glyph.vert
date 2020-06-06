#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aSize;

layout(location = 0) uniform vec2 screen;


out vec2 TexCoord;

void main() {

  vec3 pos = aPos * aSize;

  gl_Position = vec4(2 * (pos.x / screen.x) - 1.0f, 2 * (pos.y / screen.y) - 1.0f,
                     pos.z, 1.0);

  TexCoord = aTexCoord;
}
