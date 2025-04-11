#pragma once

#include <Object.h>

#include <string>

using std::string;

namespace Catalyst
{
	enum class EContentType
	{
		Invalid = -1,
		Directory,
		Texture,
		Mesh,
		Material,
		Sound,
		Shader,
		Script
	};

	class Asset : public Object
	{
	public:
		string name;
		string path;
		EContentType type;
		bool isLoaded = false;

	public:
		Asset(EContentType type);
		virtual ~Asset();

	public:
		void Load();
		void Unload();

	protected:
		virtual void OnLoad() = 0;
		virtual void OnUnload() = 0;

	};
}