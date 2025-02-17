#version 450 core

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inTex;
out vec2 texCoords;

void main() {
    texCoords = inTex;
    gl_Position = vec4(inPos, 0, 1);
}
