//  ***************************************************************
//  GraphicsModule - Creation date: 6/18/2024 9:44:38 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Application.h"

#include <string>

using std::string;

namespace Catalyst
{
	class Shader;
	class Texture;
	class Mesh;

	class ViewportCameraActor;
	class EditorWindow;

	class EditorApplication final : public Application
	{
	public:
		static int RunEditor(const char* projectPath);

	public:
		string GetProjectContentDir();

	protected:
		void OnOpened() override;
		void OnClosed() override;

		void Tick() override;
		void Render() override;

		void GenerateConfigFiles() override;

	private:
		Config* m_editorConfig;
		char* m_projectPath;

		ViewportCameraActor* m_vpCam;

		list<EditorWindow*> m_editors;

	private:
		EditorApplication(const char* projectPath);
		~EditorApplication() override;

	private:
		void InitialiseImGui();
		void ShutdownImGui() const;

		void ApplyImGuiStyle();
		void LoadRobotoFont(float fontSize);
		void SetImGuiStyle();

		void MainDockSpace();

		void SetIcon();

	};
}
