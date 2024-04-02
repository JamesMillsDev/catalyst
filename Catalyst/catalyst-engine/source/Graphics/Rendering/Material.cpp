#include <Catalyst/Graphics/Rendering/Material.hpp>

#include <Catalyst/Graphics/Rendering/Shader.hpp>
#include <Catalyst/Graphics/Rendering/Texture.hpp>
#include <Catalyst/Engine/Application.hpp>
#include <Catalyst/Gameplay/Actors/Actor.hpp>
#include <Catalyst/Gameplay/Actors/Transform.hpp>
#include <Catalyst/Graphics/Graphics.hpp>
#include <Catalyst/Graphics/Components/LightComponent.hpp>

#include <assimp/material.h>
#include <GL/glew.h>

#include <iostream>
#include <string>
#include <tuple>
#include <utility>

using std::tuple;

#define TO_STRING(var) #var

namespace Catalyst
{
	enum EDataType : char
	{
		Float,
		Color
	};

	string ColorString(const aiColor3D& _color)
	{
		return "r: " + std::to_string(_color.r) +
			" g: " + std::to_string(_color.g) +
			" b: " + std::to_string(_color.b);
	}

	Material::Material()
		: ambient{ 1 }, diffuse{ 1 }, specular{ 0 }, emissive{ 0 }, specularPower{ 1 }, opacity{ 1 }
	{

	}

	Material::Material(const Material& _other) = default;

	void Material::LoadFromAssimp(const aiMaterial* _material, const string& _folder)
	{
		tuple<const char*, EDataType, void*> values[] =
		{
			{ TO_STRING(ambient), Color, &ambient },
			{ TO_STRING(diffuse), Color, &diffuse },
			{ TO_STRING(specular), Color, &specular },
			{ TO_STRING(emissive), Color, &emissive },
			{ "shininess", Float, &specularPower },
			{ TO_STRING(opacity), Float, &opacity }
		};

		for (unsigned int i = 0; i < _material->mNumProperties; i++)
		{
			const aiMaterialProperty* prop = _material->mProperties[i];

			for (auto value : values)
			{
				const char* id = std::get<0>(value);

				if (Contains(prop->mKey, id) && std::get<1>(value) == Color)
				{
					vec3* color = static_cast<vec3*>(std::get<2>(value));
					SetProperty(_material, prop, color);
				}
				else if (Contains(prop->mKey, id) && std::get<1>(value) == Float)
				{
					float* color = static_cast<float*>(std::get<2>(value));
					SetProperty(_material, prop, color);
				}
			}
		}

		SetProperty(aiTextureType_DIFFUSE, _material, _folder, "diffuse");
		SetProperty(aiTextureType_OPACITY, _material, _folder, "alpha");
		SetProperty(aiTextureType_AMBIENT, _material, _folder, "ambient");
		SetProperty(aiTextureType_SPECULAR, _material, _folder, "specular");
		SetProperty(aiTextureType_SHININESS, _material, _folder, "specularHighlight");
		const bool flag = SetProperty(aiTextureType_NORMALS, _material, _folder, "normal");
		SetProperty(aiTextureType_DISPLACEMENT, _material, _folder, "displacement");

		// Because of obj files seeing normals as bump, if it isn't already set, we will try
		// the normal texture as a height map
		if (!flag)
			SetProperty(aiTextureType_HEIGHT, _material, _folder, "normal");
	}

	void Material::Bind()
	{
		shader->Bind();

		// set texture slots (these don't change per material)
		// ReSharper disable CppExpressionWithoutSideEffects
		shader->Set("diffuseTexture", 0);
		shader->Set("alphaTexture", 1);
		shader->Set("ambientTexture", 2);
		shader->Set("specularTexture", 3);
		shader->Set("specularHighlightTexture", 4);
		shader->Set("normalTexture", 5);
		shader->Set("displacementTexture", 6);

		shader->Set("Ka", ambient);
		shader->Set("Kd", diffuse);
		shader->Set("Ks", specular);
		shader->Set("Ke", emissive);

		shader->Set("opacity", opacity);
		shader->Set("specularPower", specularPower);
		// ReSharper restore CppExpressionWithoutSideEffects

		BindTexture(GL_TEXTURE0, "diffuse");
		BindTexture(GL_TEXTURE1, "alpha");
		BindTexture(GL_TEXTURE2, "ambient");
		BindTexture(GL_TEXTURE3, "specular");
		BindTexture(GL_TEXTURE4, "specularHighlight");
		BindTexture(GL_TEXTURE5, "normal");
		BindTexture(GL_TEXTURE6, "displacement");

		if (Graphics* graphics = Application::GetModule<Graphics>())
		{
			const list<LightComponent*> lights = graphics->Lights();

			const LightComponent* directional = nullptr;
			for(const auto& light : lights)
			{
				if(light->directional)
				{
					directional = light;
					break;
				}
			}

			if (directional)
			{
				// ReSharper disable CppExpressionWithoutSideEffects
				shader->Set("globalLight.Color", directional->RealColor());
				shader->Set("globalLight.Position", directional->direction);
				// ReSharper restore CppExpressionWithoutSideEffects
			}
			
			// ReSharper disable once CppExpressionWithoutSideEffects
			shader->Set("numLights", graphics->LightCount());
			shader->Set("AmbientColor", vec3{ 0.f, 1.f, 1.f });

			for (int i = 0, j = 0; i < graphics->LightCount(true); ++i)
			{
				const LightComponent* light = graphics->GetLight(i);
				if(light->directional)
					continue;

				string param = "lights[" + std::to_string(j++) + "].";

				// ReSharper disable CppExpressionWithoutSideEffects
				shader->Set((param + "Position").c_str(), light->GetOwner()->GetTransform()->Location());
				shader->Set((param + "Color").c_str(), light->RealColor());
				// ReSharper restore CppExpressionWithoutSideEffects
			}
		}
	}

	void Material::SetShader(shared_ptr<class Shader> _shader)
	{
		if (shader != nullptr)
			shader.reset();

		shader = std::move(_shader);
	}

	void Material::SetTexture(const string& _name, Texture* _texture)
	{
		if (m_textures.contains(_name))
			m_textures[_name].reset();

		m_textures[_name] = shared_ptr<Texture>(_texture);
	}

	// ReSharper disable once CppMemberFunctionMayBeStatic
	void Material::SetProperty(const aiMaterial* _mat, const aiMaterialProperty* _property, vec3* _color)
	{
		aiColor3D val;
		if (_mat->Get(_property->mKey.C_Str(), 0, 0, val) == aiReturn_FAILURE)
			return;

		*_color = { val.r, val.g, val.b };
	}

	// ReSharper disable once CppMemberFunctionMayBeStatic
	void Material::SetProperty(const aiMaterial* _mat, const aiMaterialProperty* _property, float* _float)
	{
		ai_real val;
		if (_mat->Get(_property->mKey.C_Str(), 0, 0, val) == aiReturn_FAILURE)
			return;

		*_float = val;
	}

	// ReSharper disable once CppMemberFunctionMayBeStatic
	bool Material::SetProperty(const aiTextureType _type, const aiMaterial* _material, const string& _path, const string& _texture)
	{
		aiString filePath;
		aiReturn state = aiReturn_FAILURE;
		unsigned int idx;

		if (!m_textures.contains(_texture))
			m_textures[_texture] = std::make_shared<Texture>();

		for (idx = 0; idx < _material->GetTextureCount(_type); idx++)
			state = _material->GetTexture(_type, idx, &filePath);

		if (idx == _material->GetTextureCount(_type) && state == aiReturn_FAILURE)
			return false;

		m_textures[_texture]->Load(string(_path + "/" + filePath.C_Str()).c_str());
		return true;
	}

	// ReSharper disable once CppMemberFunctionMayBeStatic
	bool Material::Contains(const aiString& _string, const char* _delim)
	{
		return string(_string.data).find(_delim) != string::npos;
	}

	// ReSharper disable once CppMemberFunctionMayBeStatic
	void Material::BindTexture(const uint32 _id, const shared_ptr<Texture>& _texture)
	{
		glActiveTexture(_id);

		if (_texture != nullptr && _texture->GetHandle() > 0)
			glBindTexture(GL_TEXTURE_2D, _texture->GetHandle());
		else
			glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Material::BindTexture(const uint32 _id, const string& _textureName)
	{
		shared_ptr<Texture> texture = nullptr;
		if (m_textures.contains(_textureName))
			texture = m_textures[_textureName];

		BindTexture(_id, texture);
	}
}
