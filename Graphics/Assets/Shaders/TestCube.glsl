#version 400 core

layout(points) in;
layout(triangle_strip, max_vertices = 16) out;

in Vertex {
    vec4 colour;
    vec2 texCoord;
} IN[];

out Vertex {
    vec4 colour;
    vec2 texCoord;
} OUT;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main()
{
    int i = 0;
    mat4 pvm = projMatrix * viewMatrix * modelMatrix;
    OUT.colour = IN[0].colour;

    gl_Position = gl_in[i].gl_Position;
    vec4 centerPoint = gl_Position;
    float size = 10.0;
    // +X direction is "North", -X direction is "South"
    // +Y direction is "Up",    -Y direction is "Down"
    // +Z direction is "East",  -Z direction is "West"
    //                                     N/S   U/D   E/W
    vec4 NEU = pvm * vec4( 0.1,  0.1,  0.1, 0.0) * size;
    vec4 NED = pvm * vec4( 0.1, -0.1,  0.1, 0.0) * size;
    vec4 NWU = pvm * vec4( 0.1,  0.1, -0.1, 0.0) * size;
    vec4 NWD = pvm * vec4( 0.1, -0.1, -0.1, 0.0) * size;
    vec4 SEU = pvm * vec4(-0.1,  0.1,  0.1, 0.0) * size;
    vec4 SED = pvm * vec4(-0.1, -0.1,  0.1, 0.0) * size;
    vec4 SWU = pvm * vec4(-0.1,  0.1, -0.1, 0.0) * size;
    vec4 SWD = pvm * vec4(-0.1, -0.1, -0.1, 0.0) * size;

    // Create a cube centered on the given point.
    gl_Position = centerPoint + NED;
    EmitVertex();

    gl_Position = centerPoint + NWD;
    EmitVertex();

    gl_Position = centerPoint + SWD;
    EmitVertex();

    gl_Position = centerPoint + SED;
    EmitVertex();

    gl_Position = centerPoint + SEU;
    EmitVertex();

    gl_Position = centerPoint + SWU;
    EmitVertex();

    gl_Position = centerPoint + NWU;
    EmitVertex();

    gl_Position = centerPoint + NEU;
    EmitVertex();

    gl_Position = centerPoint + NED;
    EmitVertex();

    gl_Position = centerPoint + SED;
    EmitVertex();

    gl_Position = centerPoint + SEU;
    EmitVertex();

    gl_Position = centerPoint + NEU;
    EmitVertex();

    gl_Position = centerPoint + NWU;
    EmitVertex();

    gl_Position = centerPoint + NWD;
    EmitVertex();

    gl_Position = centerPoint + SWD;
    EmitVertex();

    gl_Position = centerPoint + SWU;
    EmitVertex();

    EndPrimitive();
}