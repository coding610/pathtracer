#version 430 core

in vec2 texCoord;
out vec4 fragColor;

uniform float time;

void main() {
    float r = sin(texCoord.x * 10.0 + time * 2.0) * 0.5 + 0.5;
    float g = sin(texCoord.y * 10.0 + time * 2.0) * 0.5 + 0.5;
    float b = sin((texCoord.x + texCoord.y) * 5.0 + time * 2.0) * 0.5 + 0.5;
    
    fragColor = vec4(r, g, b, 1.0);
}
