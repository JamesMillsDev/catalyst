//  ***************************************************************
//  Material - Creation date: 6/8/2024 3:38:24 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"

#include <string>
#include <tuple>
#include <vector>

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using std::string;
using std::tuple;
using std::vector;

using glm::vec2;
using glm::vec3;
using glm::vec4;

using glm::mat2;
using glm::mat3;
using glm::mat4;

namespace Catalyst
{
	class Shader;
	class Texture;

	class DLL Material
	{
	public:
		Shader* shader;

	public:
		Material();
		explicit Material(const Material& _other);
		~Material();

	public:
		void Bind();

		void SetShader(Shader* _shader);
		void SetTexture(string _name, Texture* _texture);

		void Set(int _id, int _value) const;
		void Set(int _id, float _value) const;
		void Set(int _id, const vec2& _value) const;
		void Set(int _id, const vec3& _value) const;
		void Set(int _id, const vec4& _value) const;
		void Set(int _id, const mat2& _value) const;
		void Set(int _id, const mat3& _value) const;
		void Set(int _id, const mat4& _value) const;
		void Set(int _id, int _count, const int* _value) const;
		void Set(int _id, int _count, const float* _value) const;
		void Set(int _id, int _count, const vec2* _value) const;
		void Set(int _id, int _count, const vec3* _value) const;
		void Set(int _id, int _count, const vec4* _value) const;
		void Set(int _id, int _count, const mat2* _value) const;
		void Set(int _id, int _count, const mat3* _value) const;
		void Set(int _id, int _count, const mat4* _value) const;
		void Set(int _id, int _slot, const class Texture* _texture) const;

		bool Set(const char* _name, int _value) const;
		bool Set(const char* _name, float _value) const;
		bool Set(const char* _name, const vec2& _value) const;
		bool Set(const char* _name, const vec3& _value) const;
		bool Set(const char* _name, const vec4& _value) const;
		bool Set(const char* _name, const mat2& _value) const;
		bool Set(const char* _name, const mat3& _value) const;
		bool Set(const char* _name, const mat4& _value) const;
		bool Set(const char* _name, int _count, const int* _value) const;
		bool Set(const char* _name, int _count, const float* _value) const;
		bool Set(const char* _name, int _count, const vec2* _value) const;
		bool Set(const char* _name, int _count, const vec3* _value) const;
		bool Set(const char* _name, int _count, const vec4* _value) const;
		bool Set(const char* _name, int _count, const mat2* _value) const;
		bool Set(const char* _name, int _count, const mat3* _value) const;
		bool Set(const char* _name, int _count, const mat4* _value) const;
		bool Set(const char* _name, int _slot, const Texture* _texture) const;

	private:
		vector<tuple<string, Texture*>> m_textures;

	};
}