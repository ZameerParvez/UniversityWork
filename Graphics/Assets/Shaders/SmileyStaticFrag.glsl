#version 400 core

uniform sampler2D mainTex;
uniform sampler2D secondTex;
uniform float time;

in Vertex {
    vec4 colour;
    vec2 texCoord;
} IN;

out vec4 fragColor;

void replaceBlue() {
    vec4 smileyColour = texture(mainTex, IN.texCoord);

    if (smileyColour.r > 0.9 && smileyColour.g < 0.1 && smileyColour.b < 0.1) discard;

    if (smileyColour.b > 0.1) {
        vec2 tempTex = IN.texCoord;
        tempTex.y += time;

        fragColor = texture(secondTex, tempTex);
    } else {
        fragColor = smileyColour;
    }
}

void fade() {
    vec4 smileyColour = texture(mainTex, IN.texCoord);
    vec4 staticColour = texture(secondTex, IN.texCoord);

    fragColor = mix(smileyColour, staticColour, 0.5 + 0.5 * sin(time));
}

void testTexCoord() {
    fragColor = vec4(IN.texCoord.x, IN.texCoord.y, 0, 1);
}

void compose() {
    vec4 smileyColour = texture(mainTex, IN.texCoord);
    vec4 staticColour = texture(secondTex, IN.texCoord);

    fragColor = vec4(smileyColour.r, smileyColour.g, staticColour.b, staticColour.w);
}

void main(void) {
    replaceBlue();
}