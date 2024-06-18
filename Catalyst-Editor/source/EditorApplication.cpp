#include "EditorApplication.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

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
		: m_editorConfig{ nullptr }, m_fov{ 0 }, m_near{ 0 }, m_far{ 0 },
		m_transform{ new ActorTransform }
	{
	}

	EditorApplication::~EditorApplication()
	{
		delete m_editorConfig;
		m_editorConfig = nullptr;
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

		m_transform->TRS({ 0, 2.f, -10.f }, vec3(0), vec3(1));
		m_transform->SetRotation(
			quat(
				lookAt(m_transform->Location(), vec3(0), { 0, 1, 0 })
			)
		);

		Gizmos::Create(
			m_editorConfig->GetValue<int>("debug", "3d.maxLines"),
			m_editorConfig->GetValue<int>("debug", "3d.maxTris"),
			m_editorConfig->GetValue<int>("debug", "2d.maxLines"),
			m_editorConfig->GetValue<int>("debug", "2d.maxTris")
		);

		m_fov = m_editorConfig->GetValue<float>("viewport", "camera.fov");
		m_near = m_editorConfig->GetValue<float>("viewport", "camera.near");
		m_far = m_editorConfig->GetValue<float>("viewport", "camera.far");
	}

	void EditorApplication::OnClosed()
	{
		Application::OnClosed();
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
			const mat4 viewMat = glm::lookAt(m_transform->Location(), m_transform->Location() + m_transform->Forward(), m_transform->Up());
			const mat4 projMat = glm::perspective(Fov(), Aspect(), m_near, m_far);

			Gizmos::Draw(projMat * viewMat);
			Gizmos::Draw2D(static_cast<float>(screen->Width()), static_cast<float>(screen->Height()));
		}
	}

	void EditorApplication::GenerateConfigFiles()
	{
		Application::GenerateConfigFiles();

		m_editorConfig = new Config(GetHandle());
	}

	float EditorApplication::Fov() const
	{
		return m_fov * (glm::pi<float>() / 180.f);
	}

	float EditorApplication::Aspect() const
	{
		int w = 0;
		int h = 0;

		const auto context = glfwGetCurrentContext();
		glfwGetWindowSize(context, &w, &h);

		const float width = static_cast<float>(w);
		const float height = static_cast<float>(h);

		return width / height;
	}
}
