#version 430 core

in vec2 texCoord;
out vec4 fragColor;

void main() {
    fragColor = vec4(0.5 * (1 + texCoord.x), 0.5 * (1 + texCoord.y), 0, 1);
}
