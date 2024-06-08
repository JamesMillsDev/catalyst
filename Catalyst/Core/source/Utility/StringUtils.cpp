#include "cacorepch.h"
#include "Utility/StringUtils.h"

namespace Catalyst
{
	void StringUtils::RemoveAll(string& _string, const string& _toRemove)
	{
		while (_string.find(_toRemove) != string::npos)
		{
			const auto start = _string.begin() + static_cast<long long>(_string.find(_toRemove));
			const auto end = start + static_cast<long long>(_toRemove.length());

			_string.erase(start, end);
		}
	}

	void StringUtils::ReplaceAll(string& _string, const string& _toReplace, const string& _replacement)
	{
		while (_string.find(_toReplace) != string::npos)
		{
			const auto start = _string.begin() + static_cast<long long>(_string.find(_toReplace));
			const auto end = start + static_cast<long long>(_toReplace.length());

			_string.replace(start, end, _replacement);
		}
	}
}