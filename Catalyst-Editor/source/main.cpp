#include "EditorApplication.h"

using Catalyst::EditorApplication;

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

int main()
{
	//	//Memory Leak Check
#if _DEBUG
	//turn on the memory leak tracking tool, displays in output
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//this function will enable a break point when the program 
	//reaches a piece of memory with the matching id
	_CrtSetBreakAlloc(303);
#endif

	return EditorApplication::RunEditor();
}