#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in float width;
layout(location = 2) in float height;

void main() { 
gl_Position = vec4( 
	2 * (aPos.x/width) - 1.0f, 
	2 * (aPos.y/height) - 1.0f, 
	aPos.z, 1.0); 
}
