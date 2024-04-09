#version 410

struct Properties
{
	sampler2D diffuseTexture;
	sampler2D specularTexture;
	sampler2D normalTexture;
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float specularPower;
};

struct Input
{
	vec4 Position;
	vec4 Normal;
	vec2 TexCoord0;
	vec4 Tangent;
	vec4 BiTangent;
};

struct Output
{
	vec4 Position;
	vec3 Normal;
	vec2 TexCoord0;
	vec3 Tangent;
	vec3 BiTangent;
};

Shader Vertex
{
	void main()
	{
		Output.Position = ModelMatrix * Input.Position;
		Output.Normal = (ModelMatrix * Input.Normal).xyz;
		Output.TexCoord0 = Input.TexCoord0;
		Output.Tangent = (ModelMatrix * vec4(Input.Tangent.xyz, 0)).xyz;
		Output.BiTangent = cross(Output.Normal, Output.Tangent) * Input.Tangent.w;
		gl_Position = ProjectionViewModel * Input.Position;
	}
};

Shader Fragment
{
	struct LightInfo
	{
		vec3 Position;
		vec3 Color;
	};
	
	uniform vec3 AmbientColor;
	uniform LightInfo globalLight;

	const int MAX_LIGHTS = 4;
	uniform int numLights;
	uniform LightInfo lights[MAX_LIGHTS];

	out vec4 FragColor;

	vec3 Diffuse(vec3 direction, vec3 color, vec3 normal)
	{
		return color * max(0, dot(normal, -direction));
	}

	vec3 Specular(vec3 direction, vec3 color, vec3 normal, vec3 view)
	{
		vec3 R = reflect(direction, normal);

		float specularTerm = pow(max(0, dot(R, view)), specularPower);
		return specularTerm * color;
	}

	void main()
	{
		// Set the normal and light direction
		vec3 N = normalize(Output.Normal);
		vec3 T = normalize(Output.Tangent);
		vec3 B = normalize(Output.BiTangent);
		vec3 L = normalize(globalLight.Position);

		mat3 TBN = mat3(T,B,N);

		vec3 texDiffuse = texture(diffuseTexture, Output.TexCoord0).rgb;
		vec3 texSpecular = texture(specularTexture, Output.TexCoord0).rgb;
		vec3 texNormal = texture(normalTexture, Output.TexCoord0).rgb;

		N = normalize(TBN * (texNormal * 2 - 1));

		// Calculate the negative light direction (Lambert Term)
		float lambertTerm = max(0, min(1, dot(N, -L)));

		// Calculate the diffuse value of light from the global source
		vec3 diffuseTotal = Diffuse(L, globalLight.Color, N);

		// Calculate the view vector... 
		vec3 V = normalize(CameraPosition - Output.Position.xyz);
		// ..and the reflection vector
		vec3 R = reflect(L, N);

		// Calculate the specular value of light from the global source
		vec3 specularTotal = Specular(L, globalLight.Color, N, V);

		for (int i = 0; i < numLights; i++)
		{
			vec3 direction = Output.Position.xyz - lights[i].Position;
			float distance = length(direction);
			direction = direction / distance;

			// Set the lighting intensity with the inverse square law
			vec3 color = lights[i].Color / (distance * distance);
        
			diffuseTotal += Diffuse(direction, color, N);
			specularTotal += Specular(direction, color, N, V);
		}

		// Calculate the properties of each color type
		vec3 ambient = AmbientColor * Ka * texDiffuse;
		vec3 diffuse = diffuseTotal * Kd * texDiffuse;
		vec3 specular = specularTotal * Ks * texSpecular;

		FragColor = vec4(ambient + diffuse + specular, 1);
	}
};