#include "EditorApplication.h"

#include <format>
#include <random>

#include <glm/glm.hpp>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

#include <Actor.h>
#include <ActorTransform.h>
#include <Screen.h>
#include <GameplayModule.h>
#include <iostream>
#include <filesystem>

#include <Components/LightComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Rendering/Material.h>
#include <Rendering/Mesh.h>
#include <Rendering/Shader.h>
#include <Rendering/Texture.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <Utility/Config.h>

#include "Debug/Gizmos.h"
#include "Viewport/ViewportCameraActor.h"

#include "tinyfiledialogs.h"

#include "Windows/ContentWindow.h"
#include "Windows/EditorWindow.h"
#include "Windows/HierarchyWindow.h"
#include "Windows/InspectorWindow.h"

using std::filesystem::path;

#define EDITOR_INITIALISED

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

	EditorApplication::EditorApplication(const char* projectPath)
		: m_editorConfig{ nullptr }, m_vpCam{ nullptr }, m_projectPath{ new char[strlen(projectPath) + 1] }
	{
		strcpy_s(m_projectPath, strlen(projectPath) + 1, projectPath);
	}

	EditorApplication::~EditorApplication()
	{
		delete m_editorConfig;
		m_editorConfig = nullptr;

		delete[] m_projectPath;

		for (EditorWindow* window : m_editors)
		{
			delete window;
		}
	}

	int EditorApplication::RunEditor(const char* projectPath)
	{
		AssignInstance(new EditorApplication(projectPath));

		return Run();
	}

	string EditorApplication::GetProjectContentDir()
	{
		path prjPath = m_projectPath;

		return prjPath.parent_path().string() + "\\Content";
	}

	void EditorApplication::OnOpened()
	{
		Application::OnOpened();

		m_editorConfig->Load();
		SetIcon();

		if (GameplayModule* module = GetModule<GameplayModule>())
		{
			m_vpCam = module->SpawnActor<ViewportCameraActor>();
			m_vpCam->Initialise(m_editorConfig);
			m_vpCam->m_hideFromHierarchy = true;
		}

		InitialiseImGui();

		TickModules();

		HierarchyWindow* hierarchy = new HierarchyWindow;
		m_editors.emplace_back(hierarchy);
		m_editors.emplace_back(new InspectorWindow(hierarchy));
		m_editors.emplace_back(new ContentWindow);

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

		ShutdownImGui();

		Gizmos::Destroy();
	}

	void EditorApplication::Tick()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

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

		MainDockSpace();

		for (auto& editor : m_editors)
		{
			if (editor != nullptr)
			{
				editor->Render(this);
			}
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO();
		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void EditorApplication::GenerateConfigFiles()
	{
		Application::GenerateConfigFiles();

		m_editorConfig = new Config(GetHandle());
	}

#pragma region ImGui
	void EditorApplication::InitialiseImGui()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(GetScreen()->GetWindowPtr(), true);
		ImGui_ImplOpenGL3_Init("#version 130");

		ApplyImGuiStyle();
	}

	void EditorApplication::ShutdownImGui() const
	{
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void EditorApplication::ApplyImGuiStyle()
	{
		SetImGuiStyle();
		LoadRobotoFont(18.0f); // Adjust font size as needed
	}

	void EditorApplication::LoadRobotoFont(float _fontSize)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("resources/fonts/Roboto-Regular.ttf", _fontSize);
	}

	void EditorApplication::SetImGuiStyle()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		// Global style settings
		style.WindowRounding = 4.0f;          // Rounded corners for windows
		style.FrameRounding = 3.0f;           // Rounded corners for frames and buttons
		//style.ItemRounding = 3.0f;            // Rounded corners for individual items (buttons, inputs)
		style.ScrollbarRounding = 12.0f;      // Scrollbar rounded ends
		style.GrabRounding = 3.0f;            // Slider and other grab elements rounded
		style.PopupRounding = 4.0f;           // Popup window rounding
		style.IndentSpacing = 6.0f;           // Space between indented items
		style.ScrollbarSize = 16.0f;          // Scrollbar size
		style.FramePadding = ImVec2(4, 6);    // Padding for frame elements (buttons, inputs)
		style.WindowPadding = ImVec2(8, 8);   // Padding for window content
		style.CellPadding = ImVec2(4, 4);     // Cell padding
		style.ItemSpacing = ImVec2(8, 6);     // Spacing between items (buttons, sliders, etc.)
		style.Alpha = 1.0f;                   // Transparency (1.0 for opaque)

		// ImGui colors matching UE5 theme
		colors[ImGuiCol_Text] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);            // Light text
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);   // Disabled text
		colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.11f, 0.13f, 1.00f);       // Window background
		colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.15f, 0.17f, 1.00f);        // Child background
		colors[ImGuiCol_PopupBg] = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);        // Popup background
		colors[ImGuiCol_Border] = ImVec4(0.22f, 0.22f, 0.24f, 0.50f);         // Border color
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);   // No shadow
		colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);        // Frame background
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.30f, 0.30f, 0.32f, 1.00f); // Frame background hovered
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.36f, 0.36f, 0.38f, 1.00f);  // Frame background active
		colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);        // Title bar background
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);  // Active title bar background
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f); // Collapsed title background
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);      // Menu bar background
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);    // Scrollbar background
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.42f, 1.00f);  // Scrollbar grab
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.55f, 0.55f, 0.57f, 1.00f); // Scrollbar grab hovered
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.70f, 0.70f, 0.72f, 1.00f); // Scrollbar grab active
		colors[ImGuiCol_CheckMark] = ImVec4(0.20f, 0.55f, 0.88f, 1.00f);      // Blue check mark
		colors[ImGuiCol_SliderGrab] = ImVec4(0.20f, 0.55f, 0.88f, 1.00f);     // Slider grab
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.10f, 0.45f, 0.80f, 1.00f); // Slider grab active
		colors[ImGuiCol_Button] = ImVec4(0.20f, 0.55f, 0.88f, 1.00f);         // Button background
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.22f, 0.60f, 0.95f, 1.00f);  // Button hovered
		colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.45f, 0.80f, 1.00f);   // Button active
		colors[ImGuiCol_Header] = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);         // Header background
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.22f, 0.22f, 0.24f, 1.00f);  // Header hovered
		colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.26f, 0.28f, 1.00f);   // Header active
		colors[ImGuiCol_Separator] = ImVec4(0.22f, 0.22f, 0.24f, 1.00f);      // Separator color
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.30f, 0.30f, 0.32f, 1.00f); // Separator hovered
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.36f, 0.36f, 0.38f, 1.00f);  // Separator active
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);      // Resize grip
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.30f, 0.30f, 0.32f, 1.00f); // Resize grip hovered
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.40f, 0.42f, 1.00f); // Resize grip active
		colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);            // Tab background
		colors[ImGuiCol_TabHovered] = ImVec4(0.25f, 0.25f, 0.28f, 1.00f);      // Tab hovered
		colors[ImGuiCol_TabActive] = ImVec4(0.30f, 0.30f, 0.32f, 1.00f);      // Tab active
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);   // Tab unfocused
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.18f, 0.20f, 1.00f); // Tab unfocused active
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f); // Empty dock background

		// Disabling the down arrow on tabs
		style.TabRounding = 3.0f;               // Round the corners of tabs
		style.TabBorderSize = 0.0f;             // Remove tab border
		style.TabMinWidthForCloseButton = -1.0f; // Don't show the close button on tabs
		colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);             // Tab background color
		colors[ImGuiCol_TabHovered] = ImVec4(0.25f, 0.25f, 0.28f, 1.00f);     // Tab hovered background color
		colors[ImGuiCol_TabActive] = ImVec4(0.30f, 0.30f, 0.32f, 1.00f);      // Tab active background color
	}

	void EditorApplication::MainDockSpace()
	{
		// Get the current size of the GLFW window
		int windowWidth, windowHeight;
		glfwGetWindowSize(GetScreen()->GetWindowPtr(), &windowWidth, &windowHeight);

		// Get the position of the GLFW window (on the screen)
		int windowPosX, windowPosY;
		glfwGetWindowPos(GetScreen()->GetWindowPtr(), &windowPosX, &windowPosY);

		// Set style settings for the main dock space window
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec2 defaultPadding = style.WindowPadding; // Save the default padding

		// Adjust style settings for the main dock space window
		style.WindowPadding = ImVec2(0.0f, 0.0f); // Remove padding inside the main dock space window
		//style.FramePadding = ImVec2(0.0f, 0.0f); // Remove padding for frames inside the dock space window
		//style.WindowBorderSize = 0.0f;           // Remove borders for the main dock space window
		//style.FrameBorderSize = 0.0f;            // Remove borders for frames inside the dock space window

		// Set the window size and position to match the GLFW window's viewport position
		ImGui::SetNextWindowSize(ImVec2((float)windowWidth, (float)windowHeight));
		ImGui::SetNextWindowPos(ImVec2((float)windowPosX, (float)windowPosY));  // Set position to the current window's position

		// Begin the main window for the dock space
		ImGui::Begin("MainDockSpace", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);

		// Create the dockspace inside the window
		ImGui::DockSpace(ImGui::GetID("MainDockSpace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

		// End the main dock space window
		ImGui::End();

		// Restore the default style for other windows after the main dock space
		style.WindowPadding = defaultPadding;  // Restore the default padding
		style.FramePadding = ImVec2(4.0f, 4.0f);  // Restore default frame padding if needed
		style.WindowBorderSize = 1.0f;         // Restore default window border size
		style.FrameBorderSize = 1.0f;          // Restore default frame border size
	}
	#pragma endregion

	void EditorApplication::SetIcon()
	{
		int width, height, channels;
		unsigned char* pixels = stbi_load("resources/icon.png", &width, &height, &channels, 4);

		if (pixels == nullptr)
		{
			std::cout << "Failed to load icon!\n";
			return;
		}

		GLFWimage icon;
		icon.width = width;
		icon.height = height;
		icon.pixels = pixels;

		glfwSetWindowIcon(GetScreen()->GetWindowPtr(), 1, &icon);

		stbi_image_free(pixels);
	}

}
