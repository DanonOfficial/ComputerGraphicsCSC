#version 330 core

layout(location = 0) in vec3 particle;
layout(location = 1) in vec4 position;
layout(location = 2) in vec4 color;

out vec4 particlecolor;


uniform vec3 cameraRight;
uniform vec3 cameraUp;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    float particleSize = position.w;
    vec3 particleCenter = position.xyz;

    vec3 vertexPosition = particleCenter + cameraRight * particle.x * particleSize + cameraUp * particle.y * particleSize;

    gl_Position = projection*view* vec4(vertexPosition, 1.0f);

    particlecolor = color;
}//
//