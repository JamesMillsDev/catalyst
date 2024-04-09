#pragma once

#include <vector>
#include <string>

using std::string;
using std::vector;
using std::istream;
using std::stringstream;

namespace Catalyst
{
	class StringUtils
	{
	public:
		template<typename TYPE>
		static vector<TYPE> Split(const string& _string, char _delim, TYPE(*_converter)(const string&));

		static void RemoveAll(string& _string, const string& _toRemove);
		static void ReplaceAll(string& _string, const string& _toReplace, const string& _replacement);

	};

	template <typename TYPE>
	vector<TYPE> StringUtils::Split(const string& _string, const char _delim, TYPE(*_converter)(const string&))
	{
		vector<TYPE> values;

		string s;
		for (const char character : _string)
		{
			if (character != _delim)
			{
				s += character;
			}
			else
			{
				values.emplace_back(_converter(s));
				s.clear();
			}
		}

		if (!s.empty())
		{
			values.emplace_back(_converter(s));
			s.clear();
		}

		return values;
	}
}