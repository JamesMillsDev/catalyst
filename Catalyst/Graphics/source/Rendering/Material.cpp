#include "cagraphicspch.h"
#include "Rendering/Material.h"

#include <Actor.h>
#include <ActorTransform.h>
#include <Application.h>
#include <GraphicsModule.h>
#include <Components/LightComponent.h>
#include <Rendering/Shader.h>
#include <Rendering/Texture.h>

#include <assimp/material.h>

#include <utility>

using std::tuple;

namespace Catalyst
{
	Material::Material()
		: shader{ nullptr }
	{

	}

	Material::Material(const Material& _other) = default;

	Material::~Material()
	{
		delete shader;
		shader = nullptr;

		for (auto& [id, texture] : m_textures)
		{
			delete texture;
			texture = nullptr;
		}

		m_textures.clear();
	}

	void Material::Bind()
	{
		shader->Bind();

		for (int i = 0; i < static_cast<int>(m_textures.size()); ++i)
		{
			tuple<string, Texture*> texture = m_textures[i];

			shader->Set(std::get<0>(texture).c_str(), std::get<1>(texture)->Bind(i));
		}

		if (GraphicsModule* graphics = Application::GetModule<GraphicsModule>())
		{
			const list<LightComponent*> lights = graphics->GetFeatures<LightComponent>();

			// ReSharper disable once CppExpressionWithoutSideEffects
			shader->Set("_LightCount", static_cast<int>(lights.size()));
			shader->Set("_AmbientColor", vec3{ .5f, .5f, .5f });
			shader->Set("_AmbientIntensity", 0.01f);
			shader->Set("_FogColor", vec3{ .5f, .5f, .5f });
			shader->Set("_FogDensity", 0.002f);

			int j = 0;
			for (LightComponent* light : lights)
			{
				string param = "_Lights[" + std::to_string(j++) + "].";

				// ReSharper disable CppExpressionWithoutSideEffects
				shader->Set((param + "position").c_str(), light->Owner()->Transform()->Location());
				shader->Set((param + "direction").c_str(), light->Owner()->Transform()->Forward());
				shader->Set((param + "color").c_str(), light->color);
				shader->Set((param + "type").c_str(), (int)light->type);
				shader->Set((param + "intensity").c_str(), light->intensity);
				// ReSharper restore CppExpressionWithoutSideEffects
			}
		}
	}

	void Material::SetShader(Shader* _shader)
	{
		delete shader;

		shader = _shader;
	}

	void Material::SetTexture(string _name, Texture* _texture)
	{
		m_textures.emplace_back(_name, _texture);
	}

	void Material::Set(const int _id, const int _value) const
	{
		shader->Set(_id, _value);
	}

	void Material::Set(const int _id, const float _value) const
	{
		shader->Set(_id, _value);
	}

	void Material::Set(const int _id, const vec2& _value) const
	{
		shader->Set(_id, _value);
	}

	void Material::Set(const int _id, const vec3& _value) const
	{
		shader->Set(_id, _value);
	}

	void Material::Set(const int _id, const vec4& _value) const
	{
		shader->Set(_id, _value);
	}

	void Material::Set(const int _id, const mat2& _value) const
	{
		shader->Set(_id, _value);
	}

	void Material::Set(const int _id, const mat3& _value) const
	{
		shader->Set(_id, _value);
	}

	void Material::Set(const int _id, const mat4& _value) const
	{
		shader->Set(_id, _value);
	}

	void Material::Set(const int _id, const int _count, const int* _value) const
	{
		shader->Set(_id, _count, _value);
	}

	void Material::Set(const int _id, const int _count, const float* _value) const
	{
		shader->Set(_id, _count, _value);
	}

	void Material::Set(const int _id, const int _count, const vec2* _value) const
	{
		shader->Set(_id, _count, _value);
	}

	void Material::Set(const int _id, const int _count, const vec3* _value) const
	{
		shader->Set(_id, _count, _value);
	}

	void Material::Set(const int _id, const int _count, const vec4* _value) const
	{
		shader->Set(_id, _count, _value);
	}

	void Material::Set(const int _id, const int _count, const mat2* _value) const
	{
		shader->Set(_id, _count, _value);
	}

	void Material::Set(const int _id, const int _count, const mat3* _value) const
	{
		shader->Set(_id, _count, _value);
	}

	void Material::Set(const int _id, const int _count, const mat4* _value) const
	{
		shader->Set(_id, _count, _value);
	}

	void Material::Set(const int _id, const int _slot, const Texture* _texture) const
	{
		shader->Set(_id, _texture->Bind(_slot));
	}

	bool Material::Set(const char* _name, const int _value) const
	{
		return shader->Set(_name, _value);
	}

	bool Material::Set(const char* _name, const float _value) const
	{
		return shader->Set(_name, _value);
	}

	bool Material::Set(const char* _name, const vec2& _value) const
	{
		return shader->Set(_name, _value);
	}

	bool Material::Set(const char* _name, const vec3& _value) const
	{
		return shader->Set(_name, _value);
	}

	bool Material::Set(const char* _name, const vec4& _value) const
	{
		return shader->Set(_name, _value);
	}

	bool Material::Set(const char* _name, const mat2& _value) const
	{
		return shader->Set(_name, _value);
	}

	bool Material::Set(const char* _name, const mat3& _value) const
	{
		return shader->Set(_name, _value);
	}

	bool Material::Set(const char* _name, const mat4& _value) const
	{
		return shader->Set(_name, _value);
	}

	bool Material::Set(const char* _name, const int _count, const int* _value) const
	{
		return shader->Set(_name, _count, _value);
	}

	bool Material::Set(const char* _name, const int _count, const float* _value) const
	{
		return shader->Set(_name, _count, _value);
	}

	bool Material::Set(const char* _name, const int _count, const vec2* _value) const
	{
		return shader->Set(_name, _count, _value);
	}

	bool Material::Set(const char* _name, const int _count, const vec3* _value) const
	{
		return shader->Set(_name, _count, _value);
	}

	bool Material::Set(const char* _name, const int _count, const vec4* _value) const
	{
		return shader->Set(_name, _count, _value);
	}

	bool Material::Set(const char* _name, const int _count, const mat2* _value) const
	{
		return shader->Set(_name, _count, _value);
	}

	bool Material::Set(const char* _name, const int _count, const mat3* _value) const
	{
		return shader->Set(_name, _count, _value);
	}

	bool Material::Set(const char* _name, const int _count, const mat4* _value) const
	{
		return shader->Set(_name, _count, _value);
	}

	bool Material::Set(const char* _name, const int _slot, const Texture* _texture) const
	{
		return shader->Set(_name, _texture->Bind(_slot));
	}
}