#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoords;

uniform struct LightInfo 
{
    vec4 Position;  // Light position in eye coords.
    vec4 L;         // Diffuse and specular light intensity
    bool valid;
// We will support up to 5 lights
} Lights[5];

// This is the "sun"
uniform LightInfo Global;

//PropertyGroup("Material")
uniform struct MaterialInfo 
{
    float Rough;
    bool Metal;
    vec4 Color;
} Material;
//EndPropertyGroup()

layout(location = 0) out vec4 FragColor;

vec4 schlickFresnel(float lDotH)
{
    vec4 f0 = vec4(0.04); // Dielectrics
    if(Material.Metal)
    {
        f0 = Material.Color;
    }

    return f0 + (1 - f0) * pow(1.0 - lDotH, 5);
}

float geomSmith(float dotProd)
{
    float k = (Material.Rough + 1.0) * (Material.Rough + 1.0) / 8.0;
    float denom = dotProd * (1 - k) + k;
    return 1.0 / denom;
}

float ggxDistribution(float nDotH)
{
    float alpha2 = Material.Rough * Material.Rough * Material.Rough * Material.Rough;
    float d = (nDotH * nDotH) * (alpha2 - 1) + 1;
    return alpha2 / (3.14159265358979 * d * d);
}

vec4 microfacetModel(LightInfo light, vec3 position, vec3 n)
{
    vec4 diffuseBrdf = vec4(0.0); // metallic
    if(!Material.Metal)
    {
        diffuseBrdf = Material.Color;
    }

    vec3 l = vec3(0.0); 
    vec4 lightI = light.L;

    if(light.Position.w == 0.0) // directional Light
    {
        l = normalize(light.Position.xyz);
    }
    else
    {
        l = light.Position.xyz - position;
        float dist = length(l);
        l = normalize(l);
        lightI /= (dist * dist);
    }

    vec3 v = normalize(-position);
    vec3 h = normalize(v + l);

    float nDotH = dot(n, h);
    float lDotH = dot(l, h);
    float nDotL = max(dot(n, l), 0.0);
    float nDotV = dot(n, v);

    vec4 specBrdf = 0.25 * ggxDistribution(nDotH) * schlickFresnel(lDotH)
                        * geomSmith(nDotL) * geomSmith(nDotV);

    return (diffuseBrdf + 3.14159265358979 * specBrdf) * lightI * nDotL;
}

void main() 
{
    vec4 sum = vec4(0);
    vec3 n = normalize(Normal);

    if(Global.valid)
    {
        sum += microfacetModel(Global, Position.xyz, n);
    }

    for(int i = 0; i < 5; i++)
    {
        LightInfo light = Lights[i];
        if(light.valid)
        {
            sum += microfacetModel(light, Position, n);
        }
    }

    // Gamma
    sum = pow(sum, vec4(1.0 / 2.2));
    FragColor = vec4(sum.r, sum.g, sum.b, 1.0);
}