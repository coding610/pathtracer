#version 450 core

out vec4 fragColor;
unfirom sampler2D pathtracerTexture;
unfirom float dimFactor;
unfirom float windowWidth;
uniform float windowHeight;

void main() {
    vec4 tracerColor = texture(pathtracerTexture, gl_FragCoord.xy / vec2(windowX, windowY));
    FragColor = tracerColor * (1.0 - dimFactor);
}
