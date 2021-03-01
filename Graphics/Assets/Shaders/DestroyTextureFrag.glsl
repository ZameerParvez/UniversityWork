#version 400 core

uniform sampler2D mainTex;
uniform sampler2D secondTex;
uniform float time;
uniform float duration;

in Vertex {
    vec4 colour;
    vec2 texCoord;
} IN;

out vec4 fragColor;

void main(void) {
    vec4 colour1 = texture(mainTex, IN.texCoord);
    vec4 colour2 = texture(secondTex, IN.texCoord);

    float proportion = 1 - clamp((duration - time) / duration, 0, 1);

    fragColor = mix(colour1, colour2, proportion);
}