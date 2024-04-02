#version 400

in vec3 vPosition;
in vec3 vNormal;

struct LightInfo
{
    vec4 Position;
    vec3 Intensity;
};

uniform LightInfo globalLight;
uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ka;            // Ambient reflectivity
uniform vec3 Ks;            // Specular reflectivity
uniform float specularHighlight;    // Specular shininess factor

layout(location = 0) out vec4 FragColor;

vec3 ads()
{
    vec3 n = normalize(vNormal);
    vec3 s = normalize(vec3(globalLight.Position) - vPosition);
    vec3 v = normalize(vec3(-vPosition));
    vec3 r = reflect(-s, n);

    return globalLight.Intensity * (Ka + Kd * max(dot(s, n), 0.0) + Ks * pow(max(dot(r,v), 0.0 ), specularHighlight));
}

void main() 
{
    FragColor = vec4(ads(), 1.0);
}