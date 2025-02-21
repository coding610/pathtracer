#version 450 core

in vec2 texCoord;
out vec4 fragColor;

uniform float windowWidth;
uniform float windowHeight;
uniform float dimFactor;
uniform sampler2D pathtracerTexture;

void main() {
    vec4 color = texture(pathtracerTexture, texCoord);
    fragColor = vec4(color.rgb * dimFactor, color.a);
}
