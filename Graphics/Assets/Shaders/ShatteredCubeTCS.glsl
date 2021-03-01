#version 400 core

uniform vec2 tessLevelInner = vec2(1, 1);
uniform vec4 tessLevelOuter = vec4(1, 2, 1, 1);

layout(vertices=3) out;

//num vertices in output patch
in Vertex {
    vec4 colour; //From Vertex Shader
    vec2 texCoord; //From Vertex Shader
} IN[]; 

//Equal to size of the draw call vertex count
out Vertex {
    vec4 colour;
    vec2 texCoord;//ToEvaluationShader
} OUT[];

//Equal to the size of the layout vertex count
void main() {
    gl_TessLevelInner[0] = tessLevelInner[0];
    gl_TessLevelInner[1] = tessLevelInner[1];
    gl_TessLevelOuter[0] = tessLevelOuter[0];
    gl_TessLevelOuter[1] = tessLevelOuter[1];
    gl_TessLevelOuter[2] = tessLevelOuter[2];
    gl_TessLevelOuter[3] = tessLevelOuter[3];
    OUT[gl_InvocationID].colour=IN[gl_InvocationID].colour;
    OUT[gl_InvocationID].texCoord=IN[gl_InvocationID].texCoord;
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
