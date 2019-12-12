#version 330 core

out vec4 FragColor;
in vec2 coord;
uniform sampler1D tex;
uniform vec2 center;
uniform float scale;
uniform int iter = 20;

void main() {
    vec2 c = vec2((coord.x) * scale - center.x, (coord.y) * scale - center.y);

    int i = 0;
    vec2 z = c;
    for (; i < iter; i++) {
        z = vec2((z.x * z.x - z.y * z.y) + c.x, (z.y * z.x + z.x * z.y) + c.y);
        if ((z.x*z.x + z.y * z.y) > 2.0){
            break;
        }
    }
    if(i == iter){
        gl_FragColor = texture1D(tex,0.0);
    }else{
        gl_FragColor = texture1D(tex,i/255.0);
    }
} //Fractals