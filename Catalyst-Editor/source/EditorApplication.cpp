#include "EditorApplication.h"

#include "Actor.h"
#include "ActorTransform.h"
#include "Screen.h"
#include "Debug/Gizmos.h"
#include "Utility/Config.h"

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
		: m_editorConfig{ nullptr }, m_vpCam{ nullptr }, m_vpCamComponent{ nullptr }
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

		m_vpCam = new Actor;
		m_vpCamComponent = m_vpCam->CreateComponent<CameraComponent>();

		if(const ActorTransform* transform = m_vpCam->Transform())
		{
			transform->TRS({ 0, 2.f, -10.f }, vec3(0), vec3(1));
			transform->LookAt(vec3(0));
		}

		Gizmos::Create(
			m_editorConfig->GetValue<int>("debug", "3d.maxLines"),
			m_editorConfig->GetValue<int>("debug", "3d.maxTris"),
			m_editorConfig->GetValue<int>("debug", "2d.maxLines"),
			m_editorConfig->GetValue<int>("debug", "2d.maxTris")
		);

		m_vpCamComponent->SetFovAngle(m_editorConfig->GetValue<float>("viewport", "camera.fov"));
		m_vpCamComponent->SetNearPlane(m_editorConfig->GetValue<float>("viewport", "camera.near"));
		m_vpCamComponent->SetFarPlane(m_editorConfig->GetValue<float>("viewport", "camera.far"));
	}

	void EditorApplication::OnClosed()
	{
		Application::OnClosed();

		Gizmos::Destroy();
	}

	void EditorApplication::Tick()
	{
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
			Gizmos::Draw(m_vpCamComponent->ProjectionView());
			Gizmos::Draw2D(static_cast<float>(screen->Width()), static_cast<float>(screen->Height()));
		}
	}

	void EditorApplication::GenerateConfigFiles()
	{
		Application::GenerateConfigFiles();

		m_editorConfig = new Config(GetHandle());
	}
}
