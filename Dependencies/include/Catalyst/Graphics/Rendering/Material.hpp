#pragma once

#include <Catalyst/Catalyst.hpp>

#include <assimp/types.h>

#include <string>
#include <map>

#include <glm/vec3.hpp>

using std::string;
using std::map;

using glm::vec3;

// ReSharper disable once CppInconsistentNaming
struct aiMaterial;
// ReSharper disable once CppInconsistentNaming
struct aiMaterialProperty;
// ReSharper disable once CppInconsistentNaming
// ReSharper disable once CppForwardEnumDeclarationWithoutUnderlyingType
enum aiTextureType;  // NOLINT(clang-diagnostic-microsoft-enum-forward-reference)

namespace Catalyst
{
	class Material
	{
	public:
		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
		vec3 emissive;

		float specularPower;
		float opacity;

		shared_ptr<class Shader> shader;

	public:
		DLL Material();
		DLL explicit Material(const Material& _other);
		~Material() = default;

	public:
		DLL void LoadFromAssimp(const aiMaterial* _material, const string& _folder);
		DLL void Bind();

		DLL void SetShader(shared_ptr<class Shader> _shader);
		DLL void SetTexture(const string& _name, class Texture* _texture);

	private:
		map<string, shared_ptr<class Texture>> m_textures;

	private:
		DLL void SetProperty(const aiMaterial* _mat, const aiMaterialProperty* _property, vec3* _color);
		DLL void SetProperty(const aiMaterial* _mat, const aiMaterialProperty* _property, float* _float);
		DLL bool SetProperty(aiTextureType _type, const aiMaterial* _material, const string& _path, const string& _texture);

		DLL bool Contains(const aiString& _string, const char* _delim);
		DLL void BindTexture(uint32 _id, const shared_ptr<class Texture>& _texture);
		DLL void BindTexture(uint32 _id, const string& _textureName);

	};
}
