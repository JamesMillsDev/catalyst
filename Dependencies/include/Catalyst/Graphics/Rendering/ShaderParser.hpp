#pragma once

#include <Catalyst/Catalyst.hpp>
#include <Catalyst/Graphics/Rendering/Shader.hpp>

#include <fstream>
#include <list>
#include <map>
#include <string>
#include <vector>

using std::ifstream;
using std::list;
using std::map;
using std::pair;
using std::string;
using std::vector;

namespace Catalyst
{
	class ShaderParser
	{
	public:
		DLL static list<Shader::SubShader*> Parse(const char* _fileName);

	private:
		const char* m_fileName;

		int m_bracketIndent;
		int m_shaderVersion;
		bool m_inSubShader;

		bool m_inInputStruct;
		bool m_inOutputStruct;
		bool m_inPropertyStruct;

		Shader::EShaderStage m_currentStage;

		map<Shader::EShaderStage, pair<int, int>> m_stageLocations;
		list<Shader::SubShader*>* m_shaders;

		pair<int, int> m_inputLocations;
		pair<int, int> m_outputLocations;
		pair<int, int> m_propertyLocations;

	private:
		DLL ShaderParser(const char* _fileName, list<Shader::SubShader*>* _shaders);
		DLL ~ShaderParser();

	private:
		DLL static Shader::EShaderStage StageFromName(const char* _stageName);
		DLL static string InjectUniforms(const vector<string>& _file, const string& _code, Shader::EShaderStage _stage);

	private:
		DLL void Run();

		DLL bool Scan();
		DLL void ScanLine(const string& _line, const int& _currentLineIndex);
		DLL bool ScanForVertexStruct(const string& _line, const int& _currentLineIndex, bool& _inStruct, pair<int, int>& _structIndexes, const string& _structTitle) const;
		DLL bool ScanForPropertyStruct(const string& _line, const int& _currentLineIndex);
		DLL void ScanForShader(const string& _line, const int& _currentLineIndex);
		DLL void ScanForBrackets(const string& _line);

		DLL void ReadShaders();
		DLL Shader::SubShader* ReadShader(const vector<string>& _file, const Shader::EShaderStage& _stage);
		DLL string InjectVertexStructures(const vector<string>& _file, const string& _code, bool _injectInput) const;
		DLL string InjectProperties(const vector<string>& _file, const string& _code) const;

	};
}
