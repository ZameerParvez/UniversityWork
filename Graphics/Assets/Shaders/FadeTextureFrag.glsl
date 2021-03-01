#version 400 core

uniform sampler2D mainTex;
uniform float time;
uniform float duration;

in Vertex {
    vec4 colour;
    vec2 texCoord;
} IN;

out vec4 fragColor;

void main(void) {
    vec4 colour1 = texture(mainTex, IN.texCoord);

	float transparency = clamp((duration - time) / duration, 0, 1);

    fragColor = vec4(colour1.xyz, transparency);
}