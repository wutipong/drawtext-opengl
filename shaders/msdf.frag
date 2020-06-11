#version 460 core
out vec4 FragColor;

in vec2 TexCoord;
layout(binding=0) uniform sampler2D alphaMap;
uniform vec4 glyphColor;

void main() { 
	float alpha = texture(alphaMap, TexCoord).r;
	
	FragColor = glyphColor;
	FragColor.a = glyphColor.a * alpha;
}
