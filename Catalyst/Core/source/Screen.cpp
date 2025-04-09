#include "cacorepch.h"
#include "Screen.h"

#include "Utility/Config.h"

#include <iostream>

namespace Catalyst
{
	const int& Screen::Width() const
	{
		return m_width;
	}

	const int& Screen::Height() const
	{
		return m_height;
	}

	const string& Screen::Title()
	{
		return m_title;
	}

	const vec4& Screen::ClearColor() const
	{
		return m_clearColor;
	}

	GLFWwindow* Screen::GetWindowPtr() const
	{
		return m_window;
	}

	void Screen::SetWidth(const int _newWidth)
	{
		m_width = _newWidth;
		glfwSetWindowSize(m_window, m_width, m_height);
	}

	void Screen::SetHeight(const int _newHeight)
	{
		m_height = _newHeight;
		glfwSetWindowSize(m_window, m_width, m_height);
	}

	void Screen::SetTitle(const char* _newTitle)
	{
		m_title = _newTitle;
		glfwSetWindowTitle(m_window, m_title.c_str());
	}

	void Screen::SetClearColor(const vec4 _newColor)
	{
		m_clearColor = _newColor;
		glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
	}

	bool Screen::WindowShouldClose() const
	{
		return glfwWindowShouldClose(m_window) == GL_TRUE || m_closeWindow;
	}

	void Screen::CloseWindow()
	{
		m_closeWindow = true;
	}

	Screen::Screen()
		: m_window{ nullptr }, m_closeWindow{ false }, m_vSync{ false },
		m_width{ 0 }, m_height{ 0 }, m_clearColor{ 0, 0, 0, 0 }
	{

	}

	Screen::~Screen()
	{
		glfwDestroyWindow(m_window);
		m_window = nullptr;
	}

	int Screen::Open(Config* _config)
	{
		if (LoadValuesFrom(_config) != GLFW_TRUE)
			return GLFW_FALSE;

		/* Initialize the library */
		if (!glfwInit())
			return GLFW_FALSE;

		/* Set the OpenGL information */
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		/* Create a windowed mode window and its OpenGL context */
		m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
		if (m_window == nullptr)
		{
			glfwTerminate();
			return GLFW_FALSE;
		}

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow*, int w, int h) { glViewport(0, 0, w, h); });

		/* Make the window's context current */
		glfwMakeContextCurrent(m_window);

		/* Attempt to load OpenGL using GLAD. */
		int version = gladLoadGL(glfwGetProcAddress);
		if (version == 0) 
		{
			std::cerr << "Unable to load OpenGL functions!" << std::endl;

			glfwDestroyWindow(m_window);
			glfwTerminate();

			return GLFW_FALSE;
		}

		glfwMaximizeWindow(m_window);
		glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
		glfwSwapInterval(m_vSync ? 1 : 0);

		/* Get the renderer information */
		const GLubyte* renderer = glGetString(GL_RENDERER);
		const GLubyte* vendor = glGetString(GL_VENDOR);
		const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

		/* Get the OpenGL information */
		GLint major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);

		glEnable(GL_DEPTH_TEST);

		/* Print the information to the console window for debugging purposes. */
		std::cout << "GL Vendor            : " << vendor << "\n";
		std::cout << "GL Renderer          : " << renderer << "\n";
		std::cout << "GL Version (string)  : " << version << "\n";
		std::cout << "GL Version (integer) : " << major << "." << minor << "\n";
		std::cout << "GLSL Version         : " << glslVersion << "\n";

		return GLFW_TRUE;
	}

	void Screen::Close() const
	{
		/* Clean up the memory for the window. */
		glfwTerminate();
	}

	bool Screen::BeginFrame()
	{
		glfwPollEvents();

		if (glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) != 0)
			return false;

		glfwGetWindowSize(m_window, &m_width, &m_height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		return true;
	}

	void Screen::EndFrame() const
	{
		glfwSwapBuffers(m_window);
	}

	GLFWwindow*& Screen::Context()
	{
		return m_window;
	}

	int Screen::LoadValuesFrom(Config* _config)
	{
		if (!_config->TryGetValue<int>("window", "width", m_width))
			return GLFW_FALSE;

		if (!_config->TryGetValue<int>("window", "width", m_height))
			return GLFW_FALSE;

		if (!_config->TryGetValue<string>("application", "title", m_title))
			return GLFW_FALSE;

		if (!_config->TryGetValue<vec4>("window", "clrCol", m_clearColor))
			return GLFW_FALSE;

		if (!_config->TryGetValue<bool>("application", "vSync", m_vSync))
			return GLFW_FALSE;

		return GLFW_TRUE;
	}
}