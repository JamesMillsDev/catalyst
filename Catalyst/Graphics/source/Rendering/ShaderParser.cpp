#include "cagraphicspch.h"
#include "Rendering/ShaderParser.h"

#include <Utility/StringUtils.h>

#include <algorithm>
#include <iostream>
#include <ranges>
#include <sstream>

#include <GLFW/glfw3.h>

using std::stringstream;

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
	{ "Normal", "12" },
	{ "Color", "13" },
	{ "Color1", "14" },
	{ "Color2", "15" },
	{ "Color3", "16" },
	{ "Color4", "17" },
	{ "Color5", "18" },
	{ "Color6", "19" },
	{ "Color7", "20" }
};

namespace Catalyst
{
	list<Shader::SubShader*> ShaderParser::Parse(const char* _fileName)
	{
		list<Shader::SubShader*> shaders;

		ShaderParser* parser = new ShaderParser(_fileName, &shaders);
		parser->Run();
		delete parser;

		return shaders;
	}

	ShaderParser::ShaderParser(const char* _fileName, list<Shader::SubShader*>* _shaders)
		: m_fileName{ _fileName }, m_bracketIndent{ 0 }, m_shaderVersion{ 0 },
		m_inSubShader{ false }, m_inInputStruct{ false }, m_inOutputStruct{ false },
		m_inPropertyStruct{ false }, m_currentStage{ Shader::Undefined }, m_shaders{ _shaders }
	{
	}

	ShaderParser::~ShaderParser() = default;

	Shader::EShaderStage ShaderParser::StageFromName(const char* _stageName)
	{
		if (strcmp(_stageName, "Vertex") == 0)
			return Shader::Vertex;

		if (strcmp(_stageName, "TessEvaluation") == 0)
			return Shader::TessellationEvaluation;

		if (strcmp(_stageName, "TessControl") == 0)
			return Shader::TessellationControl;

		if (strcmp(_stageName, "Geometry") == 0)
			return Shader::Geometry;

		if (strcmp(_stageName, "Fragment") == 0)
			return Shader::Fragment;

		return Shader::Undefined;
	}

	string ShaderParser::InjectUniforms(const vector<string>& _file, const string& _code, Shader::EShaderStage _stage)
	{
		stringstream stream;

		switch (_stage)  // NOLINT(clang-diagnostic-switch-enum)
		{
		case Shader::Fragment:
			stream << "uniform vec3 CameraPosition;\n";
			break;

		case Shader::Vertex:
			stream << "uniform mat4 ProjectionViewModel;\n";
			stream << "uniform mat4 ModelMatrix;\n";
			break;

		default:
			break;
		}

		stream << _code;

		return stream.str();
	}

	void ShaderParser::Run()
	{
		if (!Scan())
			return;

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
			return;

		if (ScanForVertexStruct(_line, _currentLineIndex, m_inOutputStruct, m_outputLocations, "Output"))
			return;

		if (ScanForPropertyStruct(_line, _currentLineIndex))
			return;

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
			m_bracketIndent++;

		if (_line[0] == '}')
			m_bracketIndent--;
	}

	void ShaderParser::ReadShaders()
	{
		ifstream stream(m_fileName);

		if (stream.is_open())
		{
			vector<string> contents;
			string line;

			while (std::getline(stream, line))
				contents.emplace_back(line);

			m_shaderVersion = std::stoi(
				contents[0].substr(
					string("#version").length() + 1
				)
			);

			for (const auto& stage : m_stageLocations | std::views::keys)
				m_shaders->emplace_back(ReadShader(contents, stage));
		}

		stream.close();
	}

	Shader::SubShader* ShaderParser::ReadShader(const vector<string>& _file, const Shader::EShaderStage& _stage)
	{
		Shader::SubShader* shader = new Shader::SubShader(_stage);

		string code;

		for (int i = m_stageLocations[_stage].first - 1; i < m_stageLocations[_stage].second; ++i)
		{
			if (_file[i].find("//") == string::npos)
			{
				code += _file[i];
				code += "\n";
			}
		}

		code = InjectUniforms(_file, code, _stage);

		if (_stage == Shader::Fragment)
			code = InjectProperties(_file, code);

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

		std::cout << code << "\n";

		return shader;
	}

	string ShaderParser::InjectVertexStructures(const vector<string>& _file, const string& _code, const bool _injectInput) const
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

	string ShaderParser::InjectProperties(const vector<string>& _file, const string& _code) const
	{
		stringstream stream;

		for (int i = m_propertyLocations.first - 1; i < m_propertyLocations.second; ++i)
		{
			const string& line = _file[i];

			stream << "uniform ";
			stream << line;
			stream << "\n";
		}

		stream << _code;

		return stream.str();
	}
}
