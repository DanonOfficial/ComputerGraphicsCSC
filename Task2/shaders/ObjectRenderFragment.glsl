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
float mod289(float x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 mod289(vec4 x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 perm(vec4 x){return mod289(((x * 34.0) + 1.0) * x);}

float noise(vec3 p){
    vec3 a = floor(p);
    vec3 d = p - a;
    d = d * d * (3.0 - 2.0 * d);

    vec4 b = a.xxyy + vec4(0.0, 1.0, 0.0, 1.0);
    vec4 k1 = perm(b.xyxy);
    vec4 k2 = perm(k1.xyxy + b.zzww);

    vec4 c = k2 + a.zzzz;
    vec4 k3 = perm(c);
    vec4 k4 = perm(c + 1.0);

    vec4 o1 = fract(k3 * (1.0 / 41.0));
    vec4 o2 = fract(k4 * (1.0 / 41.0));

    vec4 o3 = o2 * d.z + o1 * (1.0 - d.z);
    vec2 o4 = o3.yw * d.x + o3.xz * (1.0 - d.x);

    return o4.y * d.y + o4.x * (1.0 - d.y);
}

void main()
{
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;
    float noiseFromCoordinate = noise(pos);
    vec3 lightDir = lightPos - pos;
    float diff = max(dot(normalize(normal), normalize(lightDir)), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 result = (ambient+diffuse) * vec3(0.6f, 0.6f, 0.6f);
    float depth = linearizeDepth(gl_FragCoord.z) / zFar;
    if(noiseFromCoordinate > threshold){
        color = vec4(result, 1-depth);
    }else {
        color = vec4(noiseFromCoordinate,noiseFromCoordinate,noiseFromCoordinate, 0.f);
    }

}
//