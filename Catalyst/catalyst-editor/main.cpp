#include <Catalyst/Engine/BaseApplication.hpp>
#include <Catalyst/Gameplay/GameInstance.hpp>

using Catalyst::BaseApplication;
using Catalyst::GameInstance;

/* Used to open an application... otherwise unused */
class EditorGame : public GameInstance
{
public:
	EditorGame() {}

};

class EditorApplication : public BaseApplication
{
public:
	EditorApplication(GameInstance* _game, const string& _appPath)
		: BaseApplication(_game, _appPath) {}

};

int main(int argc, char* argv[])
{
	return BaseApplication::Run<EditorGame, EditorApplication>(argv);
}