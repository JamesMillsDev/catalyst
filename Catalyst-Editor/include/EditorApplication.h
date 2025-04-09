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

struct Sphere;

namespace Catalyst
{
	class Shader;
	class Texture;

	class ViewportCameraActor;

	class EditorApplication final : public Application
	{
	public:
		EditorApplication();
		~EditorApplication() override;

		EditorApplication(const EditorApplication&) = delete;
		EditorApplication(EditorApplication&&) noexcept = delete;

	public:
		EditorApplication& operator=(const EditorApplication&) = delete;
		EditorApplication& operator=(EditorApplication&&) noexcept = delete;

	public:
		static int RunEditor();

	protected:
		void OnOpened() override;
		void OnClosed() override;

		void Tick() override;
		void Render() override;

		void GenerateConfigFiles() override;

	private:
		Config* m_editorConfig;

		ViewportCameraActor* m_vpCam;

		Sphere* m_sphere;
		Shader* m_shader;
		Texture* m_albedoTexture;
		Texture* m_normalTexture;

	};
}
