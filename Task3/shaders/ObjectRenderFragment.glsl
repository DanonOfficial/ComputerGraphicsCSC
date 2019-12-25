#version 330
out vec4 color;

in vec3 normal;
in vec3 pos;
in vec2 texture;
//in float gl_FragDepth;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float threshold;
float zNear = 0.1;
float zFar  = 20.0;

float linearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * zNear * zFar) / (zFar + zNear - z * (zFar - zNear));
}

void main()
{
    float ambientStrength = 0.5f;
    vec3 ambient = ambientStrength * lightColor;
    vec3 lightDir = lightPos - pos;
    float diff = max(dot(normalize(normal), normalize(lightDir)), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 result = (ambient+diffuse) * vec3(0.6f, 0.6f, 0.6f);
    float depth = linearizeDepth(gl_FragCoord.z) / zFar;
    color = vec4(result, 1.f);
}
//