#pragma once

#include <Catalyst/Catalyst.hpp>

#include <assimp/types.h>

#include <string>
#include <map>
#include <memory>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

using std::string;
using std::map;
using std::shared_ptr;

using glm::vec2;
using glm::vec3;
using glm::vec4;

using glm::mat2;
using glm::mat3;
using glm::mat4;

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

		DLL void Set(int _id, int _value) const;
		DLL void Set(int _id, float _value) const;
		DLL void Set(int _id, const vec2& _value) const;
		DLL void Set(int _id, const vec3& _value) const;
		DLL void Set(int _id, const vec4& _value) const;
		DLL void Set(int _id, const mat2& _value) const;
		DLL void Set(int _id, const mat3& _value) const;
		DLL void Set(int _id, const mat4& _value) const;
		DLL void Set(int _id, int _count, const int* _value) const;
		DLL void Set(int _id, int _count, const float* _value) const;
		DLL void Set(int _id, int _count, const vec2* _value) const;
		DLL void Set(int _id, int _count, const vec3* _value) const;
		DLL void Set(int _id, int _count, const vec4* _value) const;
		DLL void Set(int _id, int _count, const mat2* _value) const;
		DLL void Set(int _id, int _count, const mat3* _value) const;
		DLL void Set(int _id, int _count, const mat4* _value) const;

		// these calls should be avoided, but wraps up OpenGL a little
		DLL bool Set(const char* _name, int _value) const;
		DLL bool Set(const char* _name, float _value) const;
		DLL bool Set(const char* _name, const vec2& _value) const;
		DLL bool Set(const char* _name, const vec3& _value) const;
		DLL bool Set(const char* _name, const vec4& _value) const;
		DLL bool Set(const char* _name, const mat2& _value) const;
		DLL bool Set(const char* _name, const mat3& _value) const;
		DLL bool Set(const char* _name, const mat4& _value) const;
		DLL bool Set(const char* _name, int _count, const int* _value) const;
		DLL bool Set(const char* _name, int _count, const float* _value) const;
		DLL bool Set(const char* _name, int _count, const vec2* _value) const;
		DLL bool Set(const char* _name, int _count, const vec3* _value) const;
		DLL bool Set(const char* _name, int _count, const vec4* _value) const;
		DLL bool Set(const char* _name, int _count, const mat2* _value) const;
		DLL bool Set(const char* _name, int _count, const mat3* _value) const;
		DLL bool Set(const char* _name, int _count, const mat4* _value) const;

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
