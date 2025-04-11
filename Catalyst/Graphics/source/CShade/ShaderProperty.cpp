#include "cagraphicspch.h"
#include "CShade/ShaderProperty.h"

#include "Rendering/Shader.h"

#include <Utility/StringUtils.h>

#include <format>

namespace Catalyst
{
	string ShaderProperty::DisplayName()
	{
		return m_displayName;
	}

	string ShaderProperty::UniformName()
	{
		return m_uniform;
	}

	uint ShaderProperty::StageToFlag(uint _stage)
	{
		switch ((Shader::EShaderStage)_stage)
		{
		case Shader::EShaderStage::Vertex: return (uint)EStageFlag::Vertex;
		case Shader::EShaderStage::TessellationEvaluation: return (uint)EStageFlag::TessEval;
		case Shader::EShaderStage::TessellationControl: return (uint)EStageFlag::TessCtrl;
		case Shader::EShaderStage::Geometry: return (uint)EStageFlag::Geometry;
		case Shader::EShaderStage::Fragment: return (uint)EStageFlag::Fragment;
		}

		return 0;
	}

	ShaderProperty::ShaderProperty(string _uniform, const EPropertyType _type, string _displayName,
		string _initialValue, uint _stageFlags, string _customType, bool isArray, uint _arrayCount)
		: m_uniform{ std::move(_uniform) }, m_type{ _type }, m_displayName{ std::move(_displayName) },
		m_initialValue{ std::move(_initialValue) }, m_stageFlags{ _stageFlags << 1 }, m_customType{ _customType },
		m_isArray{ isArray }, m_arrayCount{ _arrayCount }
	{
	}

	bool ShaderProperty::IsValidForStage(uint _stage)
	{
		uint stage = _stage << 1;

		return (m_stageFlags & stage) == stage;
	}

	string ShaderProperty::UniformString()
	{
		return std::format("uniform {} {}{};", 
			TypeString(), m_uniform,
			(m_isArray ? std::format("[{}]", m_arrayCount) : ""));
	}

	string ShaderProperty::TypeString() const
	{
		switch (m_type)
		{
		case EPropertyType::Bool:
		{
			return "bool";
		}
		case EPropertyType::Integer:
		{
			return "int";
		}
		case EPropertyType::Float:
		{
			return "float";
		}
		case EPropertyType::Vector2:
		{
			return "vec2";
		}
		case EPropertyType::Vector3:
		{
			return "vec3";
		}
		case EPropertyType::Vector4:
		case EPropertyType::Color:
		{
			return "vec4";
		}
		case EPropertyType::Texture:
		case EPropertyType::BumpMap:
		{
			return "sampler2D";
		}
		case EPropertyType::Custom:
		{
			return m_customType;
		}
		}

		return "invalid";
	}

	void ShaderProperty::TryBindInitialValue(Shader* shader)
	{
		switch (m_type)
		{
		case EPropertyType::Bool:
		{
			shader->Set(m_uniform.c_str(), ReadInitialAsBool());
			break;
		}
		case EPropertyType::Integer:
		{
			shader->Set(m_uniform.c_str(), ReadInitialAsInteger());
			break;
		}
		case EPropertyType::Float:
		{
			shader->Set(m_uniform.c_str(), ReadInitialAsFloat());
			break;
		}
		case EPropertyType::Vector2:
		{
			shader->Set(m_uniform.c_str(), ReadInitialAsVector2());
			break;
		}
		case EPropertyType::Vector3:
		{
			shader->Set(m_uniform.c_str(), ReadInitialAsVector3());
			break;
		}
		case EPropertyType::Vector4:
		case EPropertyType::Color:
		{
			shader->Set(m_uniform.c_str(), ReadInitialAsVector4());
			break;
		}
		}
	}

	bool ShaderProperty::HasInitialValue()
	{
		return m_initialValue != "";
	}

	bool ShaderProperty::ReadInitialAsBool()
	{
		return m_initialValue == "true" ? true : false;
	}

	int ShaderProperty::ReadInitialAsInteger()
	{
		return std::stoi(m_initialValue);
	}

	float ShaderProperty::ReadInitialAsFloat()
	{
		return std::stof(m_initialValue);
	}

	vec2 ShaderProperty::ReadInitialAsVector2()
	{
		vector<float> v = StringUtils::Split<float>(m_initialValue, ',', [](const string& s) -> float
			{
				return std::stof(s);
			});

		return { v[0], v[1] };
	}

	vec3 ShaderProperty::ReadInitialAsVector3()
	{
		vector<float> v = StringUtils::Split<float>(m_initialValue, ',', [](const string& s) -> float
			{
				return std::stof(s);
			});

		return { v[0], v[1], v[2] };
	}

	vec4 ShaderProperty::ReadInitialAsVector4()
	{
		vector<float> v = StringUtils::Split<float>(m_initialValue, ',', [](const string& s) -> float
			{
				return std::stof(s);
			});

		return { v[0], v[1], v[2], v[3] };
	}
}
