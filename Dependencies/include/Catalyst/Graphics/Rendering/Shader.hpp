#pragma once

#include <Catalyst/Catalyst.hpp>

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
	class Shader
	{
		friend class Material;
		friend class ShaderParser;

	private:
		enum EShaderStage : uint32
		{
			Undefined = 0,
			Vertex,
			TessellationEvaluation,
			TessellationControl,
			Geometry,
			Fragment,
			ShaderStageCount
		};

		class SubShader
		{
			friend class Shader;
			friend class ShaderParser;

		public:
			DLL ~SubShader();

		private:
			DLL SubShader(uint32 _stage);

		private:
			DLL uint32 GetStage() const;
			DLL uint32 GetHandle() const;

			DLL const char* GetLastError() const;

		private:
			uint32 m_stage;
			uint32 m_handle;
			char* m_lastError;

		private:
			DLL void Create(const char* _source);

		};

	public:
		DLL Shader(const char* _fileName);
		DLL ~Shader();

	private:
		DLL const char* GetLastError() const;

		DLL void Bind() const;

		DLL uint32 GetHandle() const;

		DLL int GetUniform(const char* _name) const;

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
		uint32 m_program;
		shared_ptr<SubShader> m_shaders[ShaderStageCount];
		char* m_lastError;
		const char* m_fileName;

	private:
		DLL void Load();
		DLL void Attach(const shared_ptr<SubShader>& _shader);
		DLL bool Link();

	};
}