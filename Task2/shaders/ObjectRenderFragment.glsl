#version 330 core
out vec4 color;

in vec3 normal;
in vec3 pos;
//in float gl_FragDepth;
uniform vec3 lightPos;
uniform vec3 lightColor;

float zNear = 0.1;
float zFar  = 30.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * zNear * zFar) / (zFar + zNear - z * (zFar - zNear));
}



void main()
{
    // ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 lightDir = lightPos - pos;
    float diff = max(dot(normalize(normal), normalize(lightDir)), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient+diffuse) * vec3(0.6f, 0.6f, 0.6f);
    float depth = LinearizeDepth(gl_FragCoord.z) / zFar;
    color = vec4(result, 1-depth);
}