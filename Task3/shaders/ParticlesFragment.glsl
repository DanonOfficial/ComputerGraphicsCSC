#version 330 core

in vec4 particlecolor;
out vec4 color;

void main(){
    // Output color = color of the texture at the specified UV
    color = particlecolor;

}//
//