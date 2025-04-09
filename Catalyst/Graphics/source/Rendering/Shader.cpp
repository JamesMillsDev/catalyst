#include "cagraphicspch.h"
#include "Rendering/Shader.h"
#include "CShade/ShaderParser.h"
#include "CShade/ShaderProperty.h"

#include <cassert>

#include <GL/glew.h>

using std::fstream;

namespace Catalyst
{
	Shader::SubShader::~SubShader()
	{
		delete[] m_lastError;
		glDeleteShader(m_handle);
	}

	Shader::SubShader::SubShader(const uint _stage)
		: m_stage{ _stage }, m_handle{ 0 }, m_lastError{ nullptr }
	{
	}

	uint Shader::SubShader::GetStage() const
	{
		return m_stage;
	}

	uint Shader::SubShader::GetHandle() const
	{
		return m_handle;
	}

	const char* Shader::SubShader::GetLastError() const
	{
		return m_lastError;
	}

	void Shader::SubShader::Create(const char* _source)
	{
		assert(m_stage > 0 && m_stage < EShaderStage::ShaderStageCount);

		switch (m_stage)
		{
		case Vertex:
			m_handle = glCreateShader(GL_VERTEX_SHADER);
			break;
		case TessellationEvaluation:
			m_handle = glCreateShader(GL_TESS_EVALUATION_SHADER);
			break;
		case TessellationControl:
			m_handle = glCreateShader(GL_TESS_CONTROL_SHADER);
			break;
		case Geometry:
			m_handle = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		case Fragment:
			m_handle = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		default:
			m_handle = static_cast<uint>(-1);
			break;
		}

		// open file
		glShaderSource(m_handle, 1, &_source, nullptr);
		glCompileShader(m_handle);

		int success = GL_TRUE;
		glGetShaderiv(m_handle, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
		{
			int infoLogLength = 0;
			glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &infoLogLength);

			delete[] m_lastError;
			m_lastError = new char[infoLogLength];
			glGetShaderInfoLog(m_handle, infoLogLength, nullptr, m_lastError);
		}
	}

	Shader::Shader(const char* _fileName)
		: m_program{ 0 }, m_lastError{ nullptr }, m_fileName{ _fileName }
	{
		m_shaders[0] = m_shaders[1] = m_shaders[2] = m_shaders[3] = m_shaders[4] = nullptr;

		Load();
	}

	Shader::~Shader()
	{
		for (auto& shader : m_shaders)
		{
			if (shader != nullptr)
			{
				delete shader;
			}
		}

		for (auto& prop : m_properties)
		{
			delete prop;
		}

		delete[] m_lastError;
		glDeleteProgram(m_program);
	}

	const char* Shader::GetLastError() const
	{
		return m_lastError;
	}

	void Shader::Bind()
	{
		assert(m_program > 0 && "Invalid shader program");
		glUseProgram(m_program);

		for (ShaderProperty* prop : m_properties)
		{
			prop->TryBindInitialValue(this);
		}
	}

	list<ShaderProperty*> Shader::GetProperties()
	{
		return m_properties;
	}

	uint Shader::GetHandle() const
	{
		return m_program;
	}

	int Shader::GetUniform(const char* _name) const
	{
		return glGetUniformLocation(m_program, _name);
	}

	void Shader::Set(const int _id, const int _value) const
	{
		assert(m_program > 0 && "Invalid shader program");
		assert(_id >= 0 && "Invalid shader uniform");
		glUniform1i(_id, _value);
	}

	void Shader::Set(const int _id, const float _value) const
	{
		assert(m_program > 0 && "Invalid shader program");
		assert(_id >= 0 && "Invalid shader uniform");
		glUniform1f(_id, _value);
	}

	void Shader::Set(const int _id, const vec2& _value) const
	{
		assert(m_program > 0 && "Invalid shader program");
		assert(_id >= 0 && "Invalid shader uniform");
		glUniform2f(_id, _value.x, _value.y);
	}

	void Shader::Set(const int _id, const vec3& _value) const
	{
		assert(m_program > 0 && "Invalid shader program");
		assert(_id >= 0 && "Invalid shader uniform");
		glUniform3f(_id, _value.x, _value.y, _value.z);
	}

	void Shader::Set(const int _id, const vec4& _value) const
	{
		assert(m_program > 0 && "Invalid shader program");
		assert(_id >= 0 && "Invalid shader uniform");
		glUniform4f(_id, _value.x, _value.y, _value.z, _value.w);
	}

	void Shader::Set(const int _id, const mat2& _value) const
	{
		assert(m_program > 0 && "Invalid shader program");
		assert(_id >= 0 && "Invalid shader uniform");
		glUniformMatrix2fv(_id, 1, GL_FALSE, &_value[0][0]);
	}

	void Shader::Set(const int _id, const mat3& _value) const
	{
		assert(m_program > 0 && "Invalid shader program");
		assert(_id >= 0 && "Invalid shader uniform");
		glUniformMatrix3fv(_id, 1, GL_FALSE, &_value[0][0]);
	}

	void Shader::Set(const int _id, const mat4& _value) const
	{
		assert(m_program > 0 && "Invalid shader program");
		assert(_id >= 0 && "Invalid shader uniform");
		glUniformMatrix4fv(_id, 1, GL_FALSE, &_value[0][0]);
	}

	void Shader::Set(const int _id, const int _count, const int* _value) const
	{
		assert(m_program > 0 && "Invalid shader program");
		assert(_id >= 0 && "Invalid shader uniform");
		glUniform1iv(_id, _count, _value);
	}

	void Shader::Set(const int _id, const int _count, const float* _value) const
	{
		assert(m_program > 0 && "Invalid shader program");
		assert(_id >= 0 && "Invalid shader uniform");
		glUniform1fv(_id, _count, _value);
	}

	void Shader::Set(const int _id, const int _count, const vec2* _value) const
	{
		assert(m_program > 0 && "Invalid shader program");
		assert(_id >= 0 && "Invalid shader uniform");
		glUniform2fv(_id, _count, reinterpret_cast<const float*>(_value));
	}

	void Shader::Set(const int _id, const int _count, const vec3* _value) const
	{
		assert(m_program > 0 && "Invalid shader program");
		assert(_id >= 0 && "Invalid shader uniform");
		glUniform3fv(_id, _count, reinterpret_cast<const float*>(_value));
	}

	void Shader::Set(const int _id, const int _count, const vec4* _value) const
	{
		assert(m_program > 0 && "Invalid shader program");
		assert(_id >= 0 && "Invalid shader uniform");
		glUniform4fv(_id, _count, reinterpret_cast<const float*>(_value));
	}

	void Shader::Set(const int _id, const int _count, const mat2* _value) const
	{
		assert(m_program > 0 && "Invalid shader program");
		assert(_id >= 0 && "Invalid shader uniform");
		glUniformMatrix2fv(_id, _count, GL_FALSE, reinterpret_cast<const float*>(_value));
	}

	void Shader::Set(const int _id, const int _count, const mat3* _value) const
	{
		assert(m_program > 0 && "Invalid shader program");
		assert(_id >= 0 && "Invalid shader uniform");
		glUniformMatrix3fv(_id, _count, GL_FALSE, reinterpret_cast<const float*>(_value));
	}

	void Shader::Set(const int _id, const int _count, const mat4* _value) const
	{
		assert(m_program > 0 && "Invalid shader program");
		assert(_id >= 0 && "Invalid shader uniform");
		glUniformMatrix4fv(_id, _count, GL_FALSE, reinterpret_cast<const float*>(_value));
	}

	bool Shader::Set(const char* _name, const int _value) const
	{
		assert(m_program > 0 && "Invalid shader program");

		const int loc = glGetUniformLocation(m_program, _name);
		if (loc < 0)
			return false;

		glUniform1i(loc, _value);
		return true;
	}

	bool Shader::Set(const char* _name, const float _value) const
	{
		assert(m_program > 0 && "Invalid shader program");

		const int loc = glGetUniformLocation(m_program, _name);
		if (loc < 0)
			return false;

		glUniform1f(loc, _value);
		return true;
	}

	bool Shader::Set(const char* _name, const vec2& _value) const
	{
		assert(m_program > 0 && "Invalid shader program");

		const int loc = glGetUniformLocation(m_program, _name);
		if (loc < 0)
			return false;

		glUniform2f(loc, _value.x, _value.y);
		return true;
	}

	bool Shader::Set(const char* _name, const vec3& _value) const
	{
		assert(m_program > 0 && "Invalid shader program");

		const int loc = glGetUniformLocation(m_program, _name);
		if (loc < 0)
			return false;

		glUniform3f(loc, _value.x, _value.y, _value.z);
		return true;
	}

	bool Shader::Set(const char* _name, const vec4& _value) const
	{
		assert(m_program > 0 && "Invalid shader program");

		const int loc = glGetUniformLocation(m_program, _name);
		if (loc < 0)
			return false;

		glUniform4f(loc, _value.x, _value.y, _value.z, _value.w);
		return true;
	}

	bool Shader::Set(const char* _name, const mat2& _value) const
	{
		assert(m_program > 0 && "Invalid shader program");

		const int loc = glGetUniformLocation(m_program, _name);
		if (loc < 0)
			return false;

		glUniformMatrix2fv(loc, 1, GL_FALSE, &_value[0][0]);
		return true;
	}

	bool Shader::Set(const char* _name, const mat3& _value) const
	{
		assert(m_program > 0 && "Invalid shader program");

		const int loc = glGetUniformLocation(m_program, _name);
		if (loc < 0)
			return false;

		glUniformMatrix3fv(loc, 1, GL_FALSE, &_value[0][0]);
		return true;
	}

	bool Shader::Set(const char* _name, const mat4& _value) const
	{
		assert(m_program > 0 && "Invalid shader program");

		const int loc = glGetUniformLocation(m_program, _name);
		if (loc < 0)
			return false;

		glUniformMatrix4fv(loc, 1, GL_FALSE, &_value[0][0]);
		return true;
	}

	bool Shader::Set(const char* _name, const int _count, const int* _value) const
	{
		assert(m_program > 0 && "Invalid shader program");

		const int loc = glGetUniformLocation(m_program, _name);
		if (loc < 0)
			return false;

		glUniform1iv(loc, _count, _value);
		return true;
	}

	bool Shader::Set(const char* _name, const int _count, const float* _value) const
	{
		assert(m_program > 0 && "Invalid shader program");

		const int loc = glGetUniformLocation(m_program, _name);
		if (loc < 0)
			return false;

		glUniform1fv(loc, _count, _value);
		return true;
	}

	bool Shader::Set(const char* _name, const int _count, const vec2* _value) const
	{
		assert(m_program > 0 && "Invalid shader program");

		const int loc = glGetUniformLocation(m_program, _name);
		if (loc < 0)
			return false;

		glUniform2fv(loc, _count, reinterpret_cast<const float*>(_value));
		return true;
	}

	bool Shader::Set(const char* _name, const int _count, const vec3* _value) const
	{
		assert(m_program > 0 && "Invalid shader program");

		const int loc = glGetUniformLocation(m_program, _name);
		if (loc < 0)
			return false;

		glUniform3fv(loc, _count, reinterpret_cast<const float*>(_value));
		return true;
	}

	bool Shader::Set(const char* _name, const int _count, const vec4* _value) const
	{
		assert(m_program > 0 && "Invalid shader program");

		const int loc = glGetUniformLocation(m_program, _name);
		if (loc < 0)
			return false;

		glUniform4fv(loc, _count, reinterpret_cast<const float*>(_value));
		return true;
	}

	bool Shader::Set(const char* _name, const int _count, const mat2* _value) const
	{
		assert(m_program > 0 && "Invalid shader program");

		const int loc = glGetUniformLocation(m_program, _name);
		if (loc < 0)
			return false;

		glUniformMatrix2fv(loc, _count, GL_FALSE, reinterpret_cast<const float*>(_value));
		return true;
	}

	bool Shader::Set(const char* _name, const int _count, const mat3* _value) const
	{
		assert(m_program > 0 && "Invalid shader program");

		const int loc = glGetUniformLocation(m_program, _name);
		if (loc < 0)
			return false;

		glUniformMatrix3fv(loc, _count, GL_FALSE, reinterpret_cast<const float*>(_value));
		return true;
	}

	bool Shader::Set(const char* _name, const int _count, const mat4* _value) const
	{
		assert(m_program > 0 && "Invalid shader program");

		const int loc = glGetUniformLocation(m_program, _name);
		if (loc < 0)
			return false;

		glUniformMatrix4fv(loc, _count, GL_FALSE, reinterpret_cast<const float*>(_value));
		return true;
	}

	void Shader::Load()
	{
		list<SubShader*> shaders;
		ShaderParser::Parse(m_fileName, shaders, m_properties);

		for (SubShader* shader : shaders)
		{
			Attach(shader);
		}

		if (!Link())
		{
			printf("%s", m_lastError);
		}
	}

	void Shader::Attach(SubShader* _shader)
	{
		assert(_shader != nullptr);
		m_shaders[_shader->GetStage()] = _shader;
	}

	bool Shader::Link()
	{
		m_program = glCreateProgram();
		for (auto& s : m_shaders)
		{
			if (s != nullptr)
				glAttachShader(m_program, s->GetHandle());
		}
		glLinkProgram(m_program);

		int success = GL_TRUE;
		glGetProgramiv(m_program, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
		{
			int infoLogLength = 0;
			glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);

			delete[] m_lastError;
			m_lastError = new char[infoLogLength + 1];
			glGetProgramInfoLog(m_program, infoLogLength, nullptr, m_lastError);
			return false;
		}

		return true;
	}
}