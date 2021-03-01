#version 400 core

uniform mat4 modelMatrix = mat4(1.0f);
uniform mat4 viewMatrix = mat4(1.0f);
uniform mat4 projMatrix = mat4(1.0f);

uniform float time = 0.0f;
uniform float magnitude = 0.5f;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in Vertex {
	vec4 colour;
	vec2 texCoord;
} IN[];

out Vertex {
    vec4 colour;
    vec2 texCoord;
} OUT;

vec4 explode(vec4 position, vec3 normal) {
    float a = magnitude * sin(time);
    return position + vec4(normal, 0.0) * a;
}

void main () {
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 normal = normalize(cross(a, b));

	gl_Position = explode(gl_in[2].gl_Position, normal);
	OUT.texCoord = IN[2].texCoord;
	OUT.colour = IN[2].colour;
	EmitVertex();
	
	gl_Position = gl_in[1].gl_Position;
	OUT.texCoord = IN[1].texCoord;
	OUT.colour = IN[1].colour;
	EmitVertex();

	gl_Position = gl_in[0].gl_Position;
	OUT.texCoord = IN[0].texCoord;
	OUT.colour = IN[0].colour;
	EmitVertex();

    EndPrimitive();
}