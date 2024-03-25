#include <EditorApplication.hpp>

using Catalyst::BaseApplication;
using Catalyst::EditorApplication;
using Catalyst::EditorGame;

int main(int _argc, char* _argv[])
{
	return BaseApplication::Run<EditorGame, EditorApplication>(_argv);
}