#include "cagraphicspch.h"
#include "CShade/ShaderParser.h"

#include <Utility/StringUtils.h>

#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>

#include <GLFW/glfw3.h>

#include "CShade/ShaderPropertyParser.h"

using std::fstream;
using std::stringstream;

namespace Catalyst
{
	map<string, string> dataLayouts =
	{
		{ "Position", "0" },
		{ "Normal", "1" },
		{ "UV", "2" },
		{ "UV1", "3" },
		{ "UV2", "4" },
		{ "UV3", "5" },
		{ "UV4", "6" },
		{ "UV5", "7" },
		{ "UV6", "8" },
		{ "UV7", "9" },
		{ "Tangent", "10" },
		{ "BiTangent", "11" },
		{ "Color", "12" },
		{ "Color1", "13" },
		{ "Color2", "14" },
		{ "Color3", "15" },
		{ "Color4", "16" },
		{ "Color5", "17" },
		{ "Color6", "18" },
		{ "Color7", "19" }
	};

	void ShaderParser::Parse(const char* _fileName, list<Shader::SubShader*>& _shaders,
		list<ShaderProperty*>& _properties)
	{
		ShaderParser* parser = new ShaderParser(_fileName, _shaders, _properties);
		parser->Run();
		delete parser;
	}

	ShaderParser::ShaderParser(const char* _fileName, list<Shader::SubShader*>& _shaders, list<ShaderProperty*>& _properties)
		: m_fileName{ _fileName }, m_bracketIndent{ 0 }, m_shaderVersion{ 0 },
		m_inSubShader{ false }, m_inInputStruct{ false }, m_inOutputStruct{ false },
		m_inPropertyStruct{ false }, m_currentStage{ Shader::Undefined }, m_shaders{ _shaders },
		m_properties{ _properties }
	{
	}

	ShaderParser::~ShaderParser() = default;

	Shader::EShaderStage ShaderParser::StageFromName(const char* _stageName)
	{
		if (strcmp(_stageName, "Vertex") == 0)
		{
			return Shader::Vertex;
		}

		if (strcmp(_stageName, "TessEval") == 0)
		{
			return Shader::TessellationEvaluation;
		}

		if (strcmp(_stageName, "TessControl") == 0)
		{
			return Shader::TessellationControl;
		}

		if (strcmp(_stageName, "Geometry") == 0)
		{
			return Shader::Geometry;
		}

		if (strcmp(_stageName, "Fragment") == 0)
		{
			return Shader::Fragment;
		}

		return Shader::Undefined;
	}

	string ShaderParser::InjectUniforms(const vector<string>& _file, const string& _code, Shader::EShaderStage _stage)
	{
		stringstream stream;

		switch (_stage) // NOLINT(clang-diagnostic-switch-enum)
		{
		case Shader::EShaderStage::Fragment:
		{
			stream << "uniform vec3 _CameraPosition;\n";
			break;
		}

		case Shader::EShaderStage::Vertex:
		{
			stream << "uniform mat4 _ProjectionViewModel;\n";
			stream << "uniform mat4 _ModelMatrix;\n";
			break;
		}

		default:
		{
			break;
		}
		}

		stream << _code;

		return stream.str();
	}

	string ShaderParser::InjectLightUniforms(const vector<string>& _file, const string& _code, Shader::EShaderStage _stage)
	{
		stringstream stream;

		if (_stage == Shader::EShaderStage::Fragment)
		{
			stream << "struct Light\n\
			{\n\
				vec3 position;\n\
				vec3 color;\n\
				float intensity;\n\
				vec3 direction;\n\
				int type; // 0 = Point, 1 = Directional, 2 = Spot\n\
				float range;\n\
				float innerCone;\n\
				float outerCone;\n\
				float cutoff;\n\
				float outerCutoff;\n\
			};\n";

			stream << "uniform int _LightCount;\n";
			stream << "uniform Light _Lights[10];\n";
			stream << "uniform vec3 _AmbientColor;\n";
			stream << "uniform float _AmbientIntensity;\n";
			stream << "uniform vec3 _FogColor;\n";
			stream << "uniform float _FogDensity;\n";
		}

		stream << _code;

		return stream.str();
	}

	void ShaderParser::Run()
	{
		if (!Scan())
		{
			return;
		}

		ReadShaders();
	}

	bool ShaderParser::Scan()
	{
		ifstream stream(m_fileName);

		if (stream.is_open())
		{
			int lineIndex = 0;

			string line;
			while (std::getline(stream, line))
			{
				ScanLine(line, lineIndex);

				lineIndex++;
			}
		}

		stream.close();

		return !m_stageLocations.empty();
	}

	void ShaderParser::ScanLine(const string& _line, const int& _currentLineIndex)
	{
		ScanForBrackets(_line);

		if (ScanForVertexStruct(_line, _currentLineIndex, m_inInputStruct, m_inputLocations, "Input"))
		{
			return;
		}

		if (ScanForVertexStruct(_line, _currentLineIndex, m_inOutputStruct, m_outputLocations, "Output"))
		{
			return;
		}

		if (ScanForPropertyStruct(_line, _currentLineIndex))
		{
			return;
		}

		ScanForShader(_line, _currentLineIndex);
	}

	bool ShaderParser::ScanForVertexStruct(const string& _line, const int& _currentLineIndex, bool& _inStruct,
		pair<int, int>& _structIndexes, const string& _structTitle) const
	{
		if (_inStruct && m_bracketIndent == 0)
		{
			_structIndexes.second = _currentLineIndex;
			_inStruct = false;
			return true;
		}

		if (_line.find("struct " + _structTitle) != string::npos)
		{
			_structIndexes = { _currentLineIndex + 3, -1 };
			_inStruct = true;
			return true;
		}

		return false;
	}

	bool ShaderParser::ScanForPropertyStruct(const string& _line, const int& _currentLineIndex)
	{
		if (m_inPropertyStruct && m_bracketIndent == 0)
		{
			m_propertyLocations.second = _currentLineIndex;
			m_inPropertyStruct = false;
			return true;
		}

		if (_line.find("struct Properties") != string::npos)
		{
			m_propertyLocations = { _currentLineIndex + 3, -1 };
			m_inPropertyStruct = true;
			return true;
		}

		return false;
	}

	void ShaderParser::ScanForShader(const string& _line, const int& _currentLineIndex)
	{
		if (m_inSubShader && m_bracketIndent == 0)
		{
			m_inSubShader = false;
			m_stageLocations[m_currentStage].second = _currentLineIndex;
			return;
		}

		if (_line.find("Shader") != string::npos)
		{
			const Shader::EShaderStage stage = StageFromName(
				_line.substr(
					string("Shader").length() + 1
				).c_str()
			);

			m_inSubShader = true;
			m_currentStage = stage;
			m_stageLocations[stage] = { _currentLineIndex + 3, -1 };
		}
	}

	void ShaderParser::ScanForBrackets(const string& _line)
	{
		if (_line[0] == '{')
		{
			m_bracketIndent++;
		}

		if (_line[0] == '}')
		{
			m_bracketIndent--;
		}
	}

	void ShaderParser::ReadShaders()
	{
		ifstream stream(m_fileName);

		if (stream.is_open())
		{
			vector<string> contents;
			string line;

			while (std::getline(stream, line))
			{
				contents.emplace_back(line);
			}

			m_shaderVersion = std::stoi(
				contents[0].substr(
					string("#version").length() + 1
				)
			);

			ShaderPropertyParser propertyParser =
			{
				subrange
				{
					contents.begin() + m_propertyLocations.first - 1,
					contents.begin() + m_propertyLocations.second
				}
			};

			list<ShaderProperty*> properties = propertyParser.Parse();

			m_properties.assign(properties.begin(), properties.end());

			for (const auto& stage : m_stageLocations | std::views::keys)
			{
				m_shaders.emplace_back(ReadShader(contents, stage));
			}
		}

		stream.close();
	}

	Shader::SubShader* ShaderParser::ReadShader(const vector<string>& _file, const Shader::EShaderStage& _stage)
	{
		Shader::SubShader* shader = new Shader::SubShader(_stage);

		string code;

		for (int i = m_stageLocations[_stage].first - 1; i < m_stageLocations[_stage].second; ++i)
		{
			const string& line = _file[i];
			string caughtLine = line.substr(0, line.find("//"));

			code += caughtLine;
			if (caughtLine != "")
			{
				code += "\n";
			}
		}

		code = InjectLightUniforms(_file, code, _stage);
		code = InjectUniforms(_file, code, _stage);

		code = InjectProperties(code, _stage);

		code = InjectVertexStructures(_file, code, _stage == Shader::Vertex);

		stringstream stream;
		stream << "#version ";
		stream << m_shaderVersion;
		stream << "\n";
		stream << code;

		code = stream.str();

		std::erase(code, '\t');

		StringUtils::ReplaceAll(code, "Input.", "i");
		StringUtils::ReplaceAll(code, "Output.", "o");
		StringUtils::ReplaceAll(code, "texture2D", "sampler2D");

		shader->Create(code.c_str());

#if _DEBUG
		OutputCodeToFile(code, _stage);
#endif // _DEBUG

		return shader;
	}

	string ShaderParser::InjectVertexStructures(const vector<string>& _file, const string& _code,
		const bool _injectInput) const
	{
		stringstream stream;

		if (_injectInput)
		{
			for (int i = m_inputLocations.first - 1; i < m_inputLocations.second; ++i)
			{
				const string& line = _file[i];

				string varName = line.substr(line.find_first_of(' ') + 1);
				varName = varName.substr(0, varName.length() - 1);

				string varType = line.substr(0, line.find_first_of(' '));

				const string layout = dataLayouts[varName];

				stream << "layout(location=";
				stream << layout;
				stream << ") ";

				stream << "in ";
				stream << varType;
				stream << " i";
				stream << varName;
				stream << ";\n";
			}
		}

		for (int i = m_outputLocations.first - 1; i < m_outputLocations.second; ++i)
		{
			const string& line = _file[i];

			string varName = line.substr(line.find_first_of(' ') + 1);
			varName = varName.substr(0, varName.length() - 1);

			string varType = line.substr(0, line.find_first_of(' '));

			stream << "out ";
			stream << varType;
			stream << " o";
			stream << varName;
			stream << ";\n";
		}

		stream << _code;

		return stream.str();
	}

	string ShaderParser::InjectProperties(const string& _code, const Shader::EShaderStage& _stage) const
	{
		stringstream stream;

		for (ShaderProperty* prop : m_properties)
		{
			if (prop->IsValidForStage(ShaderProperty::StageToFlag(_stage)))
			{
				stream << prop->UniformString() << "\n";
			}
		}

		stream << _code;

		return stream.str();
	}

#ifdef _DEBUG
	void ShaderParser::OutputCodeToFile(const string& _code, const Shader::EShaderStage& _stage)
	{
		fstream file;

		string fileName = "";
		switch (_stage)
		{
		case Shader::EShaderStage::Vertex: fileName = "vert"; break;
		case Shader::EShaderStage::TessellationEvaluation: fileName = "tesseval"; break;
		case Shader::EShaderStage::TessellationControl: fileName = "tessc"; break;
		case Shader::EShaderStage::Geometry: fileName = "geo"; break;
		case Shader::EShaderStage::Fragment: fileName = "frag"; break;
		}

		file.open(std::format("{}.glsl", fileName).c_str(), std::ios::out);

		if (file.is_open())
		{
			vector<string> lines = StringUtils::Split<string>(_code, '\n', [](const string& line)
				{
					return line;
				});

			int indent = 0;
			for (const string& line : lines)
			{
				int indentNum = indent;

				if (line.find('{') != string::npos)
				{
					indent++;
				}
				else if (line.find('}') != string::npos)
				{
					indent--;
					indentNum = indent;
				}

				for (int i = 0; i < indentNum; ++i)
				{
					file << "\t";
				}

				file << line << "\n";
			}
		}

		file.close();
	}
#endif
}