#pragma once

#include <Catalyst/Catalyst.hpp>
#include <Catalyst/Gameplay/GameInstance.hpp>

#include <list>
#include <string>

using std::list;
using std::string;

struct GLFWwindow;

namespace Catalyst
{
	class BaseApplication
	{
	public:
		template<Derived<GameInstance> GAME, Derived<BaseApplication> APP>
		static int Run(char* _argv[]);

		DLL static const char* GetApplicationDirectory();
		DLL static void GetWindowSize(int* _w, int* _h);
		DLL static GLFWwindow* GetWindow();

	protected:
		GameInstance* m_game;
		class Screen* m_screen;
		shared_ptr<class Config> m_config;

		list<class IModule*> m_modules;

		string m_appPath;

		int m_windowWidth;
		int m_windowHeight;
		bool m_manualWindowSize;

	protected:
		DLL BaseApplication(GameInstance* _game, string _appPath);
		DLL virtual ~BaseApplication();

	protected:
		DLL virtual void OnApplicationOpened();
		DLL virtual void OnApplicationClosed();

		DLL virtual void Tick();
		DLL virtual void Render();

	private:
		DLL static shared_ptr<BaseApplication> m_appInstance;

	private:
		friend class Object;
		friend class Camera;

	private:
		DLL int Process();

		DLL void InitModules();
		DLL void CleanupModules();

	};

	template<Derived<GameInstance> GAME, Derived<BaseApplication> APP>
	inline int BaseApplication::Run(char* _argv[])
	{
		if (m_appInstance == nullptr)
		{
			const string argvStr = string(_argv[0]);
			string appPath = argvStr.substr(0, argvStr.find_last_of("\\"));

			m_appInstance = shared_ptr<BaseApplication>(new APP(new GAME(), appPath));
			return m_appInstance->Process();
		}

		return -1;
	}
}