#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

layout(location = 0) uniform vec2 screen;
layout(location = 1) uniform vec2 glyphSize;
layout(location = 2) uniform vec2 glyphPos;
layout(location = 3) uniform mat4 transform;
layout(location = 4) uniform float scale = 1.0f;

out vec2 TexCoord;

void main() {

  vec4 pos = vec4(aPos, 1.0f);
  pos = vec4(glyphSize, 1.0f, 1.0f) * pos;
  pos = pos + vec4(glyphPos, 0.0f, 0.0f);
  pos = scale * pos;

  pos = transform * pos;

  gl_Position = vec4(2 * (pos.x / screen.x) - 1.0f,
                     2 * (pos.y / screen.y) - 1.0f, pos.z, pos.w);

  TexCoord = aTexCoord;
}
