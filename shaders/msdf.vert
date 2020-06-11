#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

layout(location = 0) uniform vec2 screen;
layout(location = 1) uniform mat4 glyphTransform;
layout(location = 2) uniform mat4 transform;

out vec2 TexCoord;

void main() {

  vec4 pos = glyphTransform * vec4(aPos, 1.0f);
  pos = transform * pos;

  gl_Position = vec4(2 * (pos.x / screen.x) - 1.0f,
                     2 * (pos.y / screen.y) - 1.0f, pos.z, pos.w);

  TexCoord = aTexCoord;
}
