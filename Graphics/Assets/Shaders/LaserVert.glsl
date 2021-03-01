#version 400 core

uniform mat4 modelMatrix = mat4(1.0f);
uniform mat4 viewMatrix = mat4(1.0f);
uniform mat4 projMatrix = mat4(1.0f);

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texCoord;

out Vertex { 
    vec3 worldPos;//world space position...
    vec2 texCoord;
} OUT;

void main(void) {
    vec4 worldPos = modelMatrix * vec4(position,1.0);
    gl_Position = (projMatrix*viewMatrix) * worldPos;
    OUT.texCoord = texCoord;
    OUT.worldPos = worldPos.xyz;
    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
}
