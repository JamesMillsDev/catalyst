#include <Catalyst/Engine/Screen.hpp>

#include <iostream>

namespace Catalyst
{
	Screen::Screen()
		: m_window{ nullptr }
	{

	}

    Screen::~Screen()
    {
        m_window = nullptr;
    }

	int Screen::Open()
	{
        /* Initialize the library */
        if (!glfwInit())
            return GLFW_FALSE;

        /* Set the OpenGL information */
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        /* Create a windowed mode window and its OpenGL context */
        m_window = glfwCreateWindow(m_width, m_height, "Leviathan", NULL, NULL);
        if (m_window == nullptr)
        {
            glfwTerminate();
            return GLFW_FALSE;
        }

        /* Make the window's context current */
        glfwMaximizeWindow(m_window);
        glfwMakeContextCurrent(m_window);

        /* Attempt to load OpenGL using GLAD. */
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            std::cerr << "Unable to load OpenGL functions!" << std::endl;

            glfwDestroyWindow(m_window);
            glfwTerminate();

            return GLFW_FALSE;
        }

        /* Get the renderer information */
        const GLubyte* renderer = glGetString(GL_RENDERER);
        const GLubyte* vendor = glGetString(GL_VENDOR);
        const GLubyte* version = glGetString(GL_VERSION);
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

	void Screen::Close()
	{
        /* Clean up the memory for the window. */
        glfwTerminate();
	}

	void Screen::BeginFrame()
	{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Screen::EndFrame()
	{
        glfwSwapBuffers(m_window);
	}

	GLFWwindow*& Screen::Context()
	{
        return m_window;
	}
}
