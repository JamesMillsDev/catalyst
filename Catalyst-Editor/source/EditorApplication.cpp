#include "EditorApplication.h"

#include <iostream>

#include "Actor.h"
#include "ActorTransform.h"
#include "Screen.h"
#include "Debug/Gizmos.h"
#include "Utility/Config.h"
#include "Viewport/ViewportCameraActor.h"

namespace Catalyst
{
	HMODULE GetHandle()
	{
		HMODULE hModule = nullptr;
		GetModuleHandleEx(
			GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
			reinterpret_cast<LPCTSTR>(GetHandle), &hModule);

		return hModule;
	}

	EditorApplication::EditorApplication()
		: m_editorConfig{ nullptr }, m_vpCam{ nullptr }
	{
	}

	EditorApplication::~EditorApplication()
	{
		delete m_editorConfig;
		m_editorConfig = nullptr;

		delete m_vpCam;
		m_vpCam = nullptr;
	}

	int EditorApplication::RunEditor()
	{
		AssignInstance(new EditorApplication);

		return Run();
	}

	void EditorApplication::OnOpened()
	{
		Application::OnOpened();

		m_editorConfig->Load();

		m_vpCam = new ViewportCameraActor;
		m_vpCam->Initialise(m_editorConfig);

		Gizmos::Create(
			m_editorConfig->GetValue<int>("debug", "3d.maxLines"),
			m_editorConfig->GetValue<int>("debug", "3d.maxTris"),
			m_editorConfig->GetValue<int>("debug", "2d.maxLines"),
			m_editorConfig->GetValue<int>("debug", "2d.maxTris")
		);
	}

	void EditorApplication::OnClosed()
	{
		Application::OnClosed();

		Gizmos::Destroy();
	}

	void EditorApplication::Tick()
	{
		m_vpCam->Tick();

		const Color white = { .5f, .5f, .5f, .5f };
		const Color black = { 0, 0, 0, 1 };

		for (int i = 0; i < 201; ++i)
		{
			Gizmos::AddLine({ -100 + i, 0, 100 }, { -100 + i, 0, -100 }, i % 10 == 0 ? white : black);
			Gizmos::AddLine({ 100, 0, -100 + i }, { -100, 0, -100 + i }, i % 10 == 0 ? white : black);
		}
	}

	void EditorApplication::Render()
	{
		if (const Screen* screen = GetScreen())
		{
			Gizmos::Draw(m_vpCam->ProjectionView());
			Gizmos::Draw2D(static_cast<float>(screen->Width()), static_cast<float>(screen->Height()));
		}
	}

	void EditorApplication::GenerateConfigFiles()
	{
		Application::GenerateConfigFiles();

		m_editorConfig = new Config(GetHandle());
	}
}
