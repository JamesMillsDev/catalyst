//  ***************************************************************
//  ShaderParser - Creation date: 6/8/2024 3:35:37 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"
#include "Shader.h"

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
	class DLL ShaderParser
	{
	public:
		static list<Shader::SubShader*> Parse(const char* _fileName);

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
		ShaderParser(const char* _fileName, list<Shader::SubShader*>* _shaders);
		~ShaderParser();

	private:
		static Shader::EShaderStage StageFromName(const char* _stageName);
		static string InjectUniforms(const vector<string>& _file, const string& _code, Shader::EShaderStage _stage);

	private:
		void Run();

		bool Scan();
		void ScanLine(const string& _line, const int& _currentLineIndex);
		bool ScanForVertexStruct(const string& _line, const int& _currentLineIndex, bool& _inStruct, pair<int, int>& _structIndexes, const string& _structTitle) const;
		bool ScanForPropertyStruct(const string& _line, const int& _currentLineIndex);
		void ScanForShader(const string& _line, const int& _currentLineIndex);
		void ScanForBrackets(const string& _line);

		void ReadShaders();
		Shader::SubShader* ReadShader(const vector<string>& _file, const Shader::EShaderStage& _stage);
		string InjectVertexStructures(const vector<string>& _file, const string& _code, bool _injectInput) const;
		string InjectProperties(const vector<string>& _file, const string& _code) const;

	};
}