#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in float width;
layout(location = 3) in float height;

out vec2 TexCoord;

void main() {
  gl_Position = vec4(2 * (aPos.x / width) - 1.0f, 2 * (aPos.y / height) - 1.0f,
                     aPos.z, 1.0);

  TexCoord = aTexCoord;
}
