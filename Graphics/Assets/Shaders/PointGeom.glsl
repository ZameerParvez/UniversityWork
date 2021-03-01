#version 400 core

uniform float particleSize = 10.0f;
uniform mat4 modelMatrix = mat4(1.0f);
uniform mat4 viewMatrix = mat4(1.0f);
uniform mat4 projMatrix = mat4(1.0f);

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

void makeSquare(int i) {
    OUT.colour = IN[i].colour;
    
    // top right
    gl_Position = gl_in[i].gl_Position;
    gl_Position.x += particleSize;
    gl_Position.y += particleSize;
    OUT.texCoord = vec2(1 ,0);
    EmitVertex();
    // top left
    gl_Position = gl_in[i].gl_Position;
    gl_Position.x -= particleSize;
    gl_Position.y += particleSize;
    OUT.texCoord = vec2(0 ,0);
    EmitVertex();
    // bottom right
    gl_Position = gl_in[i].gl_Position;
    gl_Position.x += particleSize;
    gl_Position.y -= particleSize;
    OUT.texCoord = vec2(1 ,1);
    EmitVertex();
    // bottom left
    gl_Position = gl_in[i].gl_Position;
    gl_Position.x -= particleSize;
    gl_Position.y -= particleSize;
    OUT.texCoord = vec2(0 ,1);
    EmitVertex();

    EndPrimitive();
}

void makeCube(int i) {
    mat4 pvm = projMatrix * viewMatrix * modelMatrix;
    OUT.colour = IN[0].colour;
    gl_Position = gl_in[i].gl_Position;
    vec4 centerPoint = gl_Position;
    float size = 1.0;

    vec4 v1 = pvm * vec4(-1.0, 1.0, 1.0, 1.0) * size;    // Front-top-left
    vec4 v2 = pvm * vec4(1.0, 1.0, 1.0, 1.0) * size;     // Front-top-right
    vec4 v3 = pvm * vec4(1.0, -1.0, 1.0, 1.0) * size;    // Front-bottom-right
    vec4 v4 = pvm * vec4(-1.0, -1.0, 1.0, 1.0) * size;   // Front-bottom-left
    vec4 v5 = pvm * vec4(-1.0, 1.0, -1.0, 1.0) * size;   // Back-top-left
    vec4 v6 = pvm * vec4(1.0, 1.0, -1.0, 1.0) * size;    // Back-top-right
    vec4 v7 = pvm * vec4(1.0, -1.0, -1.0, 1.0) * size;   // Back-bottom-right
    vec4 v8 = pvm * vec4(-1.0, -1.0, -1.0, 1.0) * size;  // Back-bottom-left

    vec2 tc1 = vec2(0.0, 0.0);
    vec2 tc2 = vec2(0.0, 1.0);
    vec2 tc3 = vec2(1.0, 1.0);
    vec2 tc4 = vec2(1.0, 0.0);
    vec2 tc5 = vec2(1.0, 0.0);
    vec2 tc6 = vec2(1.0, 1.0);
    vec2 tc7 = vec2(0.0, 1.0);
    vec2 tc8 = vec2(0.0, 0.0);


    // Front-top-left 
    gl_Position = centerPoint + v1;
    OUT.texCoord = tc1;
    EmitVertex();
    
    // Front-top-right
    gl_Position = centerPoint + v2;
    OUT.texCoord = tc2;
    EmitVertex();
    
    // Front-bottom-left
    gl_Position = centerPoint + v4;
    OUT.texCoord = tc4;
    EmitVertex();
    
    // Front-bottom-right
    gl_Position = centerPoint + v3;
    OUT.texCoord = tc3;
    EmitVertex();
    
    // Back-bottom-right
    gl_Position = centerPoint + v7;
    OUT.texCoord = tc7;
    EmitVertex();
    
    // Front-top-right
    gl_Position = centerPoint + v2;
    OUT.texCoord = tc2;
    EmitVertex();
    
    // Back-top-right
    gl_Position = centerPoint + v6;
    OUT.texCoord = tc6;
    EmitVertex();
    
    // Front-top-left
    gl_Position = centerPoint + v1;
    OUT.texCoord = tc1;
    EmitVertex();

    // Back-top-left
    gl_Position = centerPoint + v5;
    OUT.texCoord = tc5;
    EmitVertex();

    // Front-bottom-left
    gl_Position = centerPoint + v4;
    OUT.texCoord = tc4;
    EmitVertex();
    
    // Back-bottom-left
    gl_Position = centerPoint + v8;
    OUT.texCoord = tc8;
    EmitVertex();
    
    // Back-bottom-right
    gl_Position = centerPoint + v7;
    OUT.texCoord = tc7;
    EmitVertex();
    
    // Back-top-left
    gl_Position = centerPoint + v5;
    OUT.texCoord = tc5;
    EmitVertex();

    // Back-top-right
    gl_Position = centerPoint + v6;
    OUT.texCoord = tc6;
    EmitVertex();

    EndPrimitive();
}

void main() {
    for (int i = 0; i < gl_in.length(); ++i) {
        makeCube(i);
    }
}




