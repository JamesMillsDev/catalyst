#include "cacorepch.h"
#include "CoreModule.h"

#include <string>
#include <filesystem>

#include "Application.h"

using std::string;
namespace fs = std::filesystem;

namespace Catalyst
{
    void MakeApplicationInstance()
    {
        Application::m_app = new Application;
    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
)
{
    Catalyst::MakeApplicationInstance();

    char* moduleName = new char[256];
    GetModuleFileNameA(hModule, moduleName, 256);

    string filePath = moduleName;
    filePath = filePath.substr(0, filePath.find_last_of('\\') + 1);
    for (const auto& entry : fs::directory_iterator(filePath.c_str()))
    {
        string fn = entry.path().filename().string();
        if (fn.find("catalyst") != string::npos && fn.find("core") == string::npos)
            LoadLibraryA(entry.path().string().c_str());
    }

    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	    Catalyst::CoreModule* module = new Catalyst::CoreModule;

    return TRUE;
}