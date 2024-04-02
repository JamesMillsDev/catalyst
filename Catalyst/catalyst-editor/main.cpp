#include <EditorApplication.hpp>

using Catalyst::Application;
using Catalyst::EditorApplication;
using Catalyst::EditorGame;

int main(int _argc, char* _argv[])
{
	return Application::Run<EditorGame, EditorApplication>(_argv);
}