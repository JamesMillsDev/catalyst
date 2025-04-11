//  ***************************************************************
//  ShaderProperty - Creation date: 3/25/2025 8:49:44 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"

#include <string>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using std::string;

using glm::vec2;
using glm::vec3;
using glm::vec4;

namespace Catalyst
{
	class DLL ShaderProperty
	{
		friend class Shader;
		friend class ShaderParser;
		friend class ShaderPropertyParser;

	public:
		enum class EPropertyType
		{
			None = -1,
			Bool,
			Integer,
			Float,
			Vector2,
			Vector3,
			Vector4,
			Color,
			Texture,
			BumpMap,
			Custom
		};

	private:
		enum class EStageFlag
		{
			Vertex = 0x01 << 0,
			TessEval = 0x01 << 1,
			TessCtrl = 0x01 << 2,
			Geometry = 0x01 << 3,
			Fragment = 0x01 << 4
		};

	public:
		string DisplayName();
		string UniformName();

	private:
		static uint StageToFlag(uint _stage);

	private:
		string m_uniform;
		EPropertyType m_type;
		string m_displayName;
		string m_initialValue;
		string m_customType;
		uint m_stageFlags;

		bool m_isArray;
		uint m_arrayCount;

	private:
		ShaderProperty(string _uniform, EPropertyType _type, string _displayName, string _initialValue, uint _stageFlags, string _customType, bool isArray, uint _arrayCount);

	private:
		bool IsValidForStage(uint _stage);

		string UniformString();
		string TypeString() const;

		void TryBindInitialValue(Shader* shader);
		bool HasInitialValue();

		bool ReadInitialAsBool();
		int ReadInitialAsInteger();
		float ReadInitialAsFloat();
		vec2 ReadInitialAsVector2();
		vec3 ReadInitialAsVector3();
		vec4 ReadInitialAsVector4();
		
	};
}
