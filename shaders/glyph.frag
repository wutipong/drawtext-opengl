#version 460 core
out vec4 FragColor;

in vec2 TexCoord;
layout(binding=0) uniform sampler2D tex;

void main() { 
	//FragColor = vec4(1.0f, 0, 1.0f, texture(tex, TexCoord).r);
	float alpha = texture(tex, TexCoord).r;

	FragColor = vec4(alpha, 0, 1.0f, 1.0f);
}
