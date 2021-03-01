#version 400 core

uniform mat4 modelMatrix = mat4(1.0f);
uniform mat4 viewMatrix = mat4(1.0f);
uniform mat4 projMatrix = mat4(1.0f);

uniform int numOfCubesPerSideLength = 10;
uniform float time = 0.0f;
uniform float duration = 10.0f;

layout(triangles) in;
layout(triangle_strip, max_vertices = 256) out;

in Vertex {
    vec4 colour;
    vec2 texCoord;
} IN[];

out Vertex {
    vec4 colour;
    vec2 texCoord;
} OUT;

void makeCube(mat4 pvm, vec4 basePoint, float size) {
    // vec4 v1 = pvm * vec4(-1.0, 1.0, 1.0, 1.0) * size;    // Front-top-left
    // vec4 v2 = pvm * vec4(1.0, 1.0, 1.0, 1.0) * size;     // Front-top-right
    // vec4 v3 = pvm * vec4(1.0, -1.0, 1.0, 1.0) * size;    // Front-bottom-right
    // vec4 v4 = pvm * vec4(-1.0, -1.0, 1.0, 1.0) * size;   // Front-bottom-left
    // vec4 v5 = pvm * vec4(-1.0, 1.0, -1.0, 1.0) * size;   // Back-top-left
    // vec4 v6 = pvm * vec4(1.0, 1.0, -1.0, 1.0) * size;    // Back-top-right
    // vec4 v7 = pvm * vec4(1.0, -1.0, -1.0, 1.0) * size;   // Back-bottom-right
    // vec4 v8 = pvm * vec4(-1.0, -1.0, -1.0, 1.0) * size;  // Back-bottom-left

    vec4 v1 = pvm * vec4(0, 1.0, 1.0, 1.0) * size;    // Front-top-left
    vec4 v2 = pvm * vec4(1.0, 1.0, 1.0, 1.0) * size;     // Front-top-right
    vec4 v3 = pvm * vec4(1.0, 0, 1.0, 1.0) * size;    // Front-bottom-right
    vec4 v4 = pvm * vec4(0, 0, 1.0, 1.0) * size;   // Front-bottom-left
    vec4 v5 = pvm * vec4(0, 1.0, 0, 1.0) * size;   // Back-top-left
    vec4 v6 = pvm * vec4(1.0, 1.0, 0, 1.0) * size;    // Back-top-right
    vec4 v7 = pvm * vec4(1.0, 0, 0, 1.0) * size;   // Back-bottom-right
    vec4 v8 = pvm * vec4(0, 0, 0, 1.0) * size;  // Back-bottom-left

    vec2 tc1 = vec2(0.0, 0.0);
    vec2 tc2 = vec2(0.0, 1.0);
    vec2 tc3 = vec2(1.0, 1.0);
    vec2 tc4 = vec2(1.0, 0.0);
    vec2 tc5 = vec2(1.0, 0.0);
    vec2 tc6 = vec2(1.0, 1.0);
    vec2 tc7 = vec2(0.0, 1.0);
    vec2 tc8 = vec2(0.0, 0.0);


    // Front-top-left 
    gl_Position = basePoint + v1;
    OUT.texCoord = tc1;
    EmitVertex();
    
    // Front-top-right
    gl_Position = basePoint + v2;
    OUT.texCoord = tc2;
    EmitVertex();
    
    // Front-bottom-left
    gl_Position = basePoint + v4;
    OUT.texCoord = tc4;
    EmitVertex();
    
    // Front-bottom-right
    gl_Position = basePoint + v3;
    OUT.texCoord = tc3;
    EmitVertex();
    
    // Back-bottom-right
    gl_Position = basePoint + v7;
    OUT.texCoord = tc7;
    EmitVertex();
    
    // Front-top-right
    gl_Position = basePoint + v2;
    OUT.texCoord = tc2;
    EmitVertex();
    
    // Back-top-right
    gl_Position = basePoint + v6;
    OUT.texCoord = tc6;
    EmitVertex();
    
    // Front-top-left
    gl_Position = basePoint + v1;
    OUT.texCoord = tc1;
    EmitVertex();

    // Back-top-left
    gl_Position = basePoint + v5;
    OUT.texCoord = tc5;
    EmitVertex();

    // Front-bottom-left
    gl_Position = basePoint + v4;
    OUT.texCoord = tc4;
    EmitVertex();
    
    // Back-bottom-left
    gl_Position = basePoint + v8;
    OUT.texCoord = tc8;
    EmitVertex();
    
    // Back-bottom-right
    gl_Position = basePoint + v7;
    OUT.texCoord = tc7;
    EmitVertex();
    
    // Back-top-left
    gl_Position = basePoint + v5;
    OUT.texCoord = tc5;
    EmitVertex();

    // Back-top-right
    gl_Position = basePoint + v6;
    OUT.texCoord = tc6;
    EmitVertex();

    EndPrimitive();
}

void main () {
    mat4 pvm = projMatrix * viewMatrix * modelMatrix;
    OUT.colour = IN[0].colour;

    float size = (400.0 / numOfCubesPerSideLength) / 2;
    float multiplier = 2 * numOfCubesPerSideLength;// * time;
    
    gl_Position = gl_in[0].gl_Position;
    vec4 basePoint = (gl_Position) * multiplier;// * time;
    makeCube(pvm, basePoint, size);

    
    // for (int i = 0; i < gl_in.length(); ++i) {
    //     gl_Position = gl_in[i].gl_Position;
    //     vec4 basePoint = (gl_Position) * multiplier;// * time;
    //     makeCube(pvm, basePoint, size);
    // }
}