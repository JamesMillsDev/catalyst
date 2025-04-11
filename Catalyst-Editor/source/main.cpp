#include "EditorApplication.h"

using Catalyst::EditorApplication;

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "tinyfiledialogs.h"

int main()
{
	//	//Memory Leak Check
#if _DEBUG
	//turn on the memory leak tracking tool, displays in output
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	//this function will enable a break point when the program 
	//reaches a piece of memory with the matching id
	//_CrtSetBreakAlloc(344);
#endif

	const char* filterPatterns[] = { "*.catprj" }; // File filter for .catprj files

	const char* filePath = tinyfd_openFileDialog(
		"Open Project File",   // Dialog title
		nullptr,               // Default path (nullptr = system default)
		1,                     // Number of filter patterns
		filterPatterns,        // Filter pattern array
		"Catalyst Project (*.catprj)", // Description of the filter
		0                      // Allow multiple selection (0 = single, 1 = multiple)
	);

	if (filePath)
	{
		return EditorApplication::RunEditor(filePath);
	}

	return EXIT_FAILURE;
}