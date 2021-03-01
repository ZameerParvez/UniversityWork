#version 400 core
layout(triangles, cw) in;

uniform sampler2D mainTex;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform bool isQuad = true;

// uniform float sizeFactor = 50.0f;

in Vertex { //Sent from the TCS
    vec4 colour;
    vec2 texCoord;
} IN[];

//Equal to TCS layout size

out Vertex{ //Each TES works on a single vertex!
    vec4 colour;
    vec2 texCoord;
} OUT;

vec4 QuadMixVec4(vec4 a, vec4 b, vec4 c, vec4 d){
    vec4 p0 = mix(a, c, gl_TessCoord.x);
    vec4 p1 = mix(b, d, gl_TessCoord.x);
    return mix(p0, p1, gl_TessCoord.y);
}

vec3 QuadMixVec3(vec3 a, vec3 b, vec3 c, vec3 d){
    vec3 p0 = mix(a, c, gl_TessCoord.x);
    vec3 p1 = mix(b, d, gl_TessCoord.x);
    return mix(p0, p1, gl_TessCoord.y);
}

vec2 QuadMixVec2(vec2 a, vec2 b, vec2 c, vec2 d) {
    vec2 p0 = mix(a, c, gl_TessCoord.x);
    vec2 p1 = mix(b, d, gl_TessCoord.x);
    return mix(p0, p1, gl_TessCoord.y);
}

void main() {
    vec3 combinedPos = vec3(0);
    
    if (isQuad) {
        combinedPos = QuadMixVec3(
            gl_in[0].gl_Position.xyz,
            gl_in[1].gl_Position.xyz,
            gl_in[2].gl_Position.xyz,
            gl_in[3].gl_Position.xyz
            );
        
        OUT.texCoord = QuadMixVec2(
            IN[0].texCoord,
            IN[1].texCoord,
            IN[2].texCoord,
            IN[3].texCoord
            );

        OUT.colour = QuadMixVec4(
            IN[0].colour,
            IN[1].colour,
            IN[2].colour,
            IN[3].colour
            );
    } 
    else {
        vec3 p0 = gl_TessCoord.x * gl_in[0].gl_Position.xyz;
        vec3 p1 = gl_TessCoord.y * gl_in[1].gl_Position.xyz;
        vec3 p2 = gl_TessCoord.z * gl_in[2].gl_Position.xyz;
        combinedPos = p0 + p1 + p2;

        // This is the wrong thing to do, barycentric weightings for the colours should be used instead
        OUT.texCoord = QuadMixVec2(
            IN[0].texCoord,
            IN[1].texCoord,
            IN[2].texCoord,
            IN[3].texCoord
            );

        OUT.colour = QuadMixVec4(
            IN[0].colour,
            IN[1].colour,
            IN[2].colour,
            IN[3].colour
            );
    }

    vec4 worldPos = modelMatrix * vec4(combinedPos, 1);
    gl_Position = projMatrix * viewMatrix * worldPos;
}
