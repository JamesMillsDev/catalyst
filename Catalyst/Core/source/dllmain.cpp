#include "cacorepch.h"
#include "CoreModule.h"

#include <filesystem>
#include <string>
#include <vector>

#include "Application.h"

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
	// Windows Header Files
	#include <windows.h>

	#define LOAD_LIB(PATH) LoadLibraryA(PATH)
	#define UNLOAD_LIB(HANDLE) FreeLibrary((HMODULE)m_handle)
#else
	#include <dlfnc.h>

	#define LOAD_LIB(PATH) dlopen(PATH, RTLD_LAZY)
	#define UNLOAD_LIB(HANDLE) dlclose(m_handle)
#endif

using std::runtime_error;
using std::string;
using std::vector;

namespace fs = std::filesystem;

namespace Catalyst
{
	void MakeApplicationInstance()
	{
		if (Application::m_app == nullptr)
			Application::AssignInstance(new Application);
	}

	void DestroyApplicationInstance()
	{
		delete Application::m_app;
		Application::m_app = nullptr;
	}
}

class DynamicLibrary
{
public:
	DynamicLibrary(const char* path)
	{
		m_handle = LOAD_LIB(path);

		if (m_handle == nullptr)
		{
			throw runtime_error(std::format("Failed to load library: {}", path).c_str());
		}
	}

	~DynamicLibrary()
	{
		if (m_handle != nullptr)
		{
			UNLOAD_LIB(m_handle);
		}
	}

private:
	void* m_handle;

};

vector<DynamicLibrary*> libraries;

string GetExecutablePath(void* hModule = nullptr)
{
	char path[1024];
	uint32_t size = sizeof(path);

#if _WIN32
	GetModuleFileNameA(static_cast<HMODULE>(hModule), path, size);
#elif __APPLE
	if (_NSGetExecutablePath(path, &size) != 0)
	{
		return "";
	}
#else
	ssize_t count = readlink("/proc/self/exe", path, size - 1);
	if (count != -1)
	{
		path[count] = '\0';
	}
	else
	{
		return "";
	}
#endif

	return path;
}

bool IsValidExtension(string ext, vector<string>& extensions)
{
	for (string& extension : extensions)
	{
		if (ext == extension)
		{
			return true;
		}
	}

	return false;
}

void Initialise(vector<string> extensions, void* hModule = nullptr)
{
	Catalyst::MakeApplicationInstance();

	string filePath = GetExecutablePath();
	filePath = filePath.substr(0, filePath.find_last_of('\\') + 1);
	filePath = filePath.substr(0, filePath.find_last_of('\\') + 1);
	for (const auto& entry : fs::directory_iterator(filePath.c_str()))
	{
		string fileName = entry.path().filename().string();
		string extension = entry.path().extension().string();

		if (fileName.find("catalyst") != string::npos && 
			fileName.find("core") == string::npos &&
			IsValidExtension(extension, extensions))
		{
			libraries.emplace_back(new DynamicLibrary(entry.path().string().c_str()));
		}
	}

	Catalyst::CoreModule* module = new Catalyst::CoreModule;
}

void Shutdown()
{
	for (DynamicLibrary* lib : libraries)
	{
		delete lib;
	}

	libraries.clear();

	Catalyst::DestroyApplicationInstance();
}

#if _WIN32
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		Initialise({ ".dll" }, hModule);
	}

	if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		Shutdown();
	}

	return TRUE;
}
#else
#include <stdio.h>

__attribute__((constructor)) void onLibraryLoad()
{
	Initialise({ ".so", ".dylib" });
}

__attribute__((destructor)) void onLibraryUnloaded()
{
	Shutdown();
}
#endif // _WIN32

