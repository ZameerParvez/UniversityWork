#version 400 core

uniform vec2 tessLevelInner = vec2(32, 32);
uniform vec4 tessLevelOuter = vec4(32, 32, 32, 32);

layout(vertices=4) out;

//num vertices in output patch
in Vertex {
    vec2 texCoord; //From Vertex Shader
} IN[]; 

//Equal to size of the draw call vertex count
out Vertex {
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
    OUT[gl_InvocationID].texCoord=IN[gl_InvocationID].texCoord;
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
