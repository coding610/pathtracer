#version 450 core

in vec2 texCoords;
out vec4 fragColor;

uniform sampler2D renderedTexture;

void main() {
    fragColor = vec4(texture(renderedTexture, texCoords).rgb * 0.9, 1);
}
