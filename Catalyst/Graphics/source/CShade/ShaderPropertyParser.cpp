#include "cagraphicspch.h"
#include "CShade/ShaderPropertyParser.h"

#include "Rendering/Shader.h"
#include "CShade/ShaderProperty.h"

#include <Utility/StringUtils.h>

namespace Catalyst
{
    ShaderPropertyParser::ShaderPropertyParser(const subrange<vector<string>::const_iterator>& _properties)
	    : m_properties{ _properties }
    {
    }

    list<ShaderProperty*> ShaderPropertyParser::Parse()
    {
        list<ShaderProperty*> properties;

        for(const string& line : m_properties)
        {
            if (line.empty() || line.find("//") != string::npos)
            {
                continue;
            }

            properties.emplace_back(ParseLine(line));
        }

	    return properties;
    }

    ShaderProperty* ShaderPropertyParser::ParseLine(const string& _line)
    {
        string propertyLine = _line;
        bool hasCustomType = false;

        uint shaderStages = 0;

        if (HasTag(propertyLine, "Vertex"))
        {
            shaderStages |= (uint)ShaderProperty::EStageFlag::Vertex;
        }

        if (HasTag(propertyLine, "TessEval"))
        {
            shaderStages |= (uint)ShaderProperty::EStageFlag::TessEval;
        }

        if (HasTag(propertyLine, "TessCtrl"))
        {
            shaderStages |= (uint)ShaderProperty::EStageFlag::TessCtrl;
        }

        if (HasTag(propertyLine, "Geometry"))
        {
            shaderStages |= (uint)ShaderProperty::EStageFlag::Geometry;
        }

        if (HasTag(propertyLine, "Fragment"))
        {
            shaderStages |= (uint)ShaderProperty::EStageFlag::Fragment;
        }

        if (shaderStages == 0)
        {
            shaderStages = (uint)ShaderProperty::EStageFlag::Fragment;
        }

        if (HasTag(propertyLine, "CustomType"))
        {
            hasCustomType = true;
        }

        string customType = "";
        bool isArray = false;
        uint arraySize = 0;

        string uniformName = GetUniformFromString(propertyLine);
        string displayName = GetDisplayNameFromString(propertyLine);
        ShaderProperty::EPropertyType type = GetTypeFromString(propertyLine, customType, hasCustomType, isArray, arraySize);
        string initialValue = GetInitialValueFromString(propertyLine, type);

	    return new ShaderProperty(uniformName, type, displayName, initialValue, shaderStages, customType, isArray, arraySize);
    }

    string ShaderPropertyParser::GetUniformFromString(string& _line)
    {
        size_t uniformStart = _line.find_first_of('_');
        size_t uniformEnd = _line.find_first_of('(');

        string uniformName = _line.substr(uniformStart, uniformEnd - uniformStart);
        StringUtils::ReplaceAll(uniformName, " ", "");

        _line.erase(0, uniformEnd);

        return uniformName;
    }

    string ShaderPropertyParser::GetDisplayNameFromString(string& _line)
    {
        size_t displayNameStart = _line.find_first_of('\"') + 1;
        size_t displayNameEnd = _line.find_first_of('\"', displayNameStart);

        string displayName = _line.substr(displayNameStart, displayNameEnd - displayNameStart);

        _line.erase(0, displayNameEnd + 1);
        StringUtils::ReplaceAll(_line, " ", "");

        size_t firstNonComma = _line.find_first_not_of(',');
        _line.erase(0, firstNonComma);

        return displayName;
    }

    ShaderProperty::EPropertyType ShaderPropertyParser::GetTypeFromString(
        string& _line, string& _customType, 
        bool _hasCustomType, bool& _isArray, uint& _arrSize)
    {
        size_t typeNameStart = 0;
        size_t typeNameEnd = _line.find_first_of(')');

        string typeName = _line.substr(typeNameStart, typeNameEnd - typeNameStart);

        size_t arrayStart = typeName.find_first_of('[');
        if (arrayStart != string::npos)
        {
            size_t arrayEnd = typeName.find_first_of(']');

            string arraySize = typeName.substr(arrayStart + 1, arrayEnd - arrayStart - 1);
            typeName.erase(arrayStart, arrayEnd - arrayStart + 1);

            _isArray = true;
            _arrSize = std::stoi(arraySize);
        }

        if (_hasCustomType)
        {
            _customType = typeName;
            return ShaderProperty::EPropertyType::Custom;
        }

        if (typeName == "bool")
        {
            return ShaderProperty::EPropertyType::Bool;
        }

        if (typeName == "int")
        {
            return ShaderProperty::EPropertyType::Integer;
        }

        if (typeName == "float")
        {
            return ShaderProperty::EPropertyType::Float;
        }

        if (typeName == "Vector2")
        {
            return ShaderProperty::EPropertyType::Vector2;
        }

        if (typeName == "Vector3")
        {
            return ShaderProperty::EPropertyType::Vector3;
        }

        if (typeName == "Vector4")
        {
            return ShaderProperty::EPropertyType::Vector4;
        }

        if (typeName == "Color")
        {
            return ShaderProperty::EPropertyType::Color;
        }

        if (typeName == "2D")
        {
            return _line.find("bump") != string::npos ? 
                ShaderProperty::EPropertyType::BumpMap :
                ShaderProperty::EPropertyType::Texture;
        }

	    return ShaderProperty::EPropertyType::None;
    }

    string ShaderPropertyParser::GetInitialValueFromString(string& _line, ShaderProperty::EPropertyType _type)
    {
        if (_type == ShaderProperty::EPropertyType::BumpMap ||
            _type == ShaderProperty::EPropertyType::Texture)
        {
            return "";
        }

        size_t initialValueStart = _line.find_first_of('=');
        size_t initialValueEnd = _line.size();

        if (initialValueStart == string::npos)
        {
            return "";
        }

        initialValueStart++;

        string initialValue = _line.substr(initialValueStart, initialValueEnd - initialValueStart);
        StringUtils::ReplaceAll(initialValue, "(", "");
        StringUtils::ReplaceAll(initialValue, ")", "");

        return initialValue;
    }

    bool ShaderPropertyParser::HasTag(string& _line, const string& _tag)
    {
        size_t tagStart = _line.find(_tag);

        if (tagStart == string::npos)
        {
            return false;
        }

        size_t tagEnd = _tag.length();

        string tag = _line.substr(tagStart, tagEnd);

        if (tag == _tag)
        {
            _line.erase(tagStart - 1, tagEnd + 2);

            return true;
        }

        return false;
    }

    bool ShaderPropertyParser::IsArray(string& _line, uint& _size)
    {
        return false;
    }
}
