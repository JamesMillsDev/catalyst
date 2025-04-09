#version 410

struct Properties
{
	texture2D albedo;
	texture2D normalMap;
	vec3 lightDir;
	vec4 tint;
}

struct Input
{
	vec4 Position;
	vec3 Normal;
	vec2 UV;
	vec3 Tangent;
	vec3 BiTangent;
}

struct Output
{
	vec4 FragColor;
}

Shader Vertex
{
	out vec2 TexCoords;
	out vec3 FragPos;
	out mat3 TBN;

	void main()
	{
		// Apply ModelMatrix and ProjectionViewModel transformations
		vec4 worldPosition = ModelMatrix * Input.Position;
		gl_Position = ProjectionViewModel * worldPosition;

		TexCoords = Input.UV;
		FragPos = worldPosition.xyz;

		vec3 T = normalize(Input.Tangent);
		vec3 B = normalize(Input.BiTangent);
		vec3 N = normalize(Input.Normal);
		TBN = mat3(T, B, N);
	}
}

Shader Fragment
{
	in vec2 TexCoords;
	in vec3 FragPos;
	in mat3 TBN;

	void main()
	{
		// Sample texture and normal map
		vec3 albedoColor = texture(albedo, TexCoords).rgb;
		vec3 normalMapColor = texture(normalMap, TexCoords).rgb * 2.0 - 1.0;

		// Transform normal from tangent space to world space
		vec3 normal = normalize(TBN * normalMapColor);
		
		// Simple diffuse lighting
		float diffuse = max(dot(normal, normalize(lightDir)), 0.0);
		vec3 lighting = albedoColor * diffuse;

		// Apply tint
		vec3 tintedColor = lighting * tint.rgb;

		Output.FragColor = vec4(tintedColor, 1.0);
	}
}
