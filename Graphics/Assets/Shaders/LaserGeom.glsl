#version 400 core

uniform mat4 modelMatrix = mat4(1.0f);
uniform mat4 viewMatrix = mat4(1.0f);
uniform mat4 projMatrix = mat4(1.0f);

uniform float magnitude = 100.0f;
uniform float speed = 20.0f;
uniform float time = 0.0f;
uniform float duration = 10.0f;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in Vertex {
	vec3 worldPos;
	vec2 texCoord;
} IN[];

out Vertex {
	vec3 worldPos;
    vec2 texCoord;
	vec3 normal;
} OUT;

void main () {
	OUT.worldPos = IN[0].worldPos;
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	OUT.normal = normalize(cross(a, b));

	for (int i = 0; i < gl_in.length(); ++i) {
		OUT.texCoord = IN[i].texCoord;
		EmitVertex();
	}

    EndPrimitive();
}