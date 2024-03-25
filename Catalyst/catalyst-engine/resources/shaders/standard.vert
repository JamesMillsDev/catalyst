#version 460

layout (location = 0) in vec4 VertexPosition;
layout (location = 1) in vec4 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;
layout (location = 3) in vec4 VertexTangent;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main()
{
	Normal = normalize(NormalMatrix * VertexNormal.xyz).xyz;
	Position = (ModelViewMatrix * vec4(VertexPosition.xyz,1.0)).xyz;
	TexCoords = VertexTexCoord;
	gl_Position = MVP * vec4(VertexPosition.xyz,1.0);
}