#version 410

struct Properties
{
	_Albedo ("Albedo", 2D) = "texture"
	_NormalMap ("Normal Map", 2D) = "bump"
	_ORM ("ORM", 2D) = "texture"
	_Tint ("Tint Color", Color) = (1, 1, 1, 1)
};

struct Input
{
	vec4 Position;
	vec3 Normal;
	vec2 UV;
	vec3 Tangent;
	vec3 BiTangent;
};

struct Output
{
	vec4 FragColor;
};

Shader Vertex
{
	out vec2 TexCoords;
	out vec3 FragPos;
	out mat3 TBN;

	void main()
	{
		// Transform position to clip space
		gl_Position = _ProjectionViewModel * _ModelMatrix * Input.Position;

		// Pass texture coordinates and world space position to fragment shader
		TexCoords = Input.UV;
		FragPos = (_ModelMatrix * Input.Position).xyz;

		// Create TBN matrix for normal map transformation
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

	const float PI = 3.14159265359;

	vec3 CalculateLighting(vec3 normal, vec3 albedo, float roughness, float metallic)
	{
		vec3 finalColor = vec3(0.0);
		vec3 viewDir = normalize(_CameraPosition - FragPos);

		// Ambient Lighting
		vec3 ambient = _AmbientColor * _AmbientIntensity * albedo;
		finalColor += ambient;

		for (int i = 0; i < _LightCount; i++)
		{
			Light light = _Lights[i];
			vec3 lightDir;
			vec3 radiance;

			if (light.type == 0) // Directional Light
			{
				lightDir = normalize(-light.direction);
				radiance = light.color * light.intensity;
			}
			else if (light.type == 1) // Point Light
			{
				lightDir = normalize(light.position - FragPos);
				float distance = length(light.position - FragPos);
				float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
				radiance = light.color * light.intensity * attenuation;
			}
			else if (light.type == 2) // Spotlight
			{
				lightDir = normalize(light.position - FragPos);
				float theta = dot(lightDir, normalize(-light.direction));
				float epsilon = light.cutoff - light.outerCutoff;
				float spotFactor = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

				float distance = length(light.position - FragPos);
				float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
				radiance = light.color * light.intensity * attenuation * spotFactor;
			}

			// Diffuse shading
			float NdotL = max(dot(normal, lightDir), 0.0);
			vec3 diffuse = radiance * albedo * NdotL;

			// Specular (Cook-Torrance BRDF)
			vec3 halfwayDir = normalize(lightDir + viewDir);
			float NdotH = max(dot(normal, halfwayDir), 0.0);
			float roughness2 = roughness * roughness;
			float D = (roughness2) / (PI * pow(NdotH * NdotH * (roughness2 - 1.0) + 1.0, 2.0));
			vec3 specular = D * radiance * metallic;

			finalColor += diffuse + specular;
		}

		return finalColor;
	}

	void main()
	{
		// Apply gamma correction to albedo (Convert sRGB to Linear)
		vec3 albedoColor = pow(texture(_Albedo, TexCoords).rgb, vec3(2.2));

		// Sample normal map (DirectX format) and convert to OpenGL format
		vec3 normalMapColor = texture(_NormalMap, TexCoords).rgb * 2.0 - 1.0;
		normalMapColor.g *= -1.0; // Flip Y (green) channel for OpenGL

		// Extract ORM channels
		vec3 ormValues = texture(_ORM, TexCoords).rgb;
		float ao = ormValues.r;       // Ambient Occlusion (Red)
		float roughness = ormValues.g; // Roughness (Green)
		float metallic = ormValues.b;  // Metallic (Blue)

		roughness = max(roughness, 0.05); // Prevents full roughness blackout

		// Transform normal to world space
		vec3 normal = normalize(TBN * normalMapColor);

		// Final lighting (apply ambient occlusion)
		vec3 lighting = CalculateLighting(normal, albedoColor, roughness, metallic) * ao;

		// Apply ambient light
		vec3 ambientLight = vec3(0);
		vec3 finalColor = (ambientLight + lighting) * _Tint.rgb;

		// Compute fog factor in the fragment shader (Exponential Fog)
		float distance = length(_CameraPosition - FragPos);
		float fogFactor = clamp(exp(-(_FogDensity * distance) * (_FogDensity * distance)), 0.0, 1.0);

		// Apply fog
		vec3 fogColor = _FogColor.rgb;
		finalColor = mix(fogColor, finalColor, fogFactor); // Blend between fog and final color

		// Apply inverse gamma correction for final output (Linear to sRGB)
		finalColor = pow(finalColor, vec3(1.0 / 2.2));

		// Final color output
		Output.FragColor = vec4(finalColor, 1.0);
	}
}