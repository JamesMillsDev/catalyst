// Our phong shader
#version 410
layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
layout(location = 2) in vec2 TexCoord;
layout(location = 3) in vec4 Tangent;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;
out vec3 vTangent;
out vec3 vBiTangent;

uniform mat4 MVP;
uniform mat4 ModelViewMatrix; // To transform the normal

void main()
{
    vPosition = vec3(ModelViewMatrix * Position);
    vNormal = (ModelViewMatrix * Normal).xyz;
    vTexCoord = TexCoord;
    vTangent = (ModelViewMatrix * Tangent).xyz;
    //vTangent = (ModelMatrix * vec4(Tangent.xyz, 0)).xyz;
    vBiTangent = cross(vNormal, vTangent) * Tangent.w;
    gl_Position = MVP * Position;
}