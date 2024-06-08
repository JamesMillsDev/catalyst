//  ***************************************************************
//  Shader - Creation date: 6/8/2024 3:37:56 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

using glm::vec2;
using glm::vec3;
using glm::vec4;

using glm::mat2;
using glm::mat3;
using glm::mat4;

namespace Catalyst
{
	class DLL Shader
	{
		friend class Material;
		friend class ShaderParser;

	private:
		enum EShaderStage : uint
		{
			Undefined = 0,
			Vertex,
			TessellationEvaluation,
			TessellationControl,
			Geometry,
			Fragment,
			ShaderStageCount
		};

		class DLL SubShader
		{
			friend class Shader;
			friend class ShaderParser;

		public:
			~SubShader();

		private:
			SubShader(uint _stage);

		private:
			uint GetStage() const;
			uint GetHandle() const;

			const char* GetLastError() const;

		private:
			uint m_stage;
			uint m_handle;
			char* m_lastError;

		private:
			void Create(const char* _source);

		};

	public:
		Shader(const char* _fileName);
		~Shader();

	private:
		const char* GetLastError() const;

		void Bind() const;

		uint GetHandle() const;

		int GetUniform(const char* _name) const;

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

		// these calls should be avoided, but wraps up OpenGL a little
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

	private:
		uint m_program;
		SubShader* m_shaders[ShaderStageCount];
		char* m_lastError;
		const char* m_fileName;

	private:
		void Load();
		void Attach(SubShader* _shader);
		bool Link();

	};
}