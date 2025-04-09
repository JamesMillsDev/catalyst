#include "EditorApplication.h"

#include <iostream>

#include <glm/glm.hpp>
#include <Rendering/Shader.h>
#include <Rendering/Texture.h>

#include "Actor.h"
#include "ActorTransform.h"
#include "Screen.h"
#include "Debug/Gizmos.h"
#include "Utility/Config.h"
#include "Viewport/ViewportCameraActor.h"

#include <vector>
#include <cmath>
#include <GL/glew.h>

struct Sphere {
    std::vector<glm::vec4> positions;  // Position as vec4
    std::vector<glm::vec3> normals;    // Normal as vec3
    std::vector<glm::vec2> uvs;        // UVs as vec2
    std::vector<glm::vec3> tangents;  // Tangent as vec3
    std::vector<glm::vec3> bitangents; // Bitangent as vec3
    unsigned int sectorCount;
    unsigned int stackCount;
    GLuint VAO, VBO, tangentVBO, bitangentVBO, uvVBO;

    Sphere(unsigned int sectorCount = 36, unsigned int stackCount = 18)
        : sectorCount(sectorCount), stackCount(stackCount) {
        generateSphereWithTangents();
        setupOpenGLBuffers();
    }

    ~Sphere() {
        // Cleanup VBO/VAO resources
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &tangentVBO);
        glDeleteBuffers(1, &bitangentVBO);
        glDeleteBuffers(1, &uvVBO);
    }

    // Function to generate sphere data (positions, normals, tangents, bitangents, uvs)
    void generateSphereWithTangents() {
        const float PI = 3.14159265359f;

        for (unsigned int i = 0; i <= stackCount; ++i) {
            float stackAngle = PI / 2 - i * PI / stackCount; // latitude angle
            float xy = cosf(stackAngle); // cos(pitch)
            float z = sinf(stackAngle); // sin(pitch)

            for (unsigned int j = 0; j <= sectorCount; ++j) {
                float sectorAngle = j * 2 * PI / sectorCount; // longitude angle
                float x = xy * cosf(sectorAngle); // X coordinate
                float y = xy * sinf(sectorAngle); // Y coordinate
                float vertexX = x; // position.x
                float vertexY = y; // position.y
                float vertexZ = z; // position.z

                // Push position as vec4 (adding 1.0f for homogeneous coordinates)
                positions.push_back(glm::vec4(vertexX, vertexY, vertexZ, 1.0f));

                // Normals: same as the vertex position (converted to vec3 with w = 0)
                normals.push_back(glm::vec3(vertexX, vertexY, vertexZ));

                // UVs: Calculate based on spherical coordinates
                float u = (float)j / sectorCount;
                float v = (float)i / stackCount;
                uvs.push_back(glm::vec2(u, v));

                // Tangents: Calculate tangent using partial derivatives of position wrt phi (the azimuthal angle)
                glm::vec3 tangent = glm::vec3(-sinf(sectorAngle), cosf(sectorAngle), 0.0f);
                tangents.push_back(glm::normalize(tangent));

                // Bi-Tangents: Cross product of normal and tangent (converted to vec3)
                glm::vec3 bitangent = glm::cross(normals.back(), tangents.back());
                bitangents.push_back(glm::normalize(bitangent));
            }
        }
    }

    // Function to setup VBO and VAO for the sphere
    void setupOpenGLBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &tangentVBO);
        glGenBuffers(1, &bitangentVBO);
        glGenBuffers(1, &uvVBO);

        // Bind the VAO
        glBindVertexArray(VAO);

        // Bind and send positions to VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec4), &positions[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
        glEnableVertexAttribArray(0); // Position - "0"

        // Bind and send normals to VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glEnableVertexAttribArray(1); // Normal - "1"

        // Bind and send UVs to VBO
        glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
        glEnableVertexAttribArray(2); // UV - "2"

        // Bind and send tangents to VBO
        glBindBuffer(GL_ARRAY_BUFFER, tangentVBO);
        glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), &tangents[0], GL_STATIC_DRAW);
        glVertexAttribPointer(10, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glEnableVertexAttribArray(10); // Tangent - "10"

        // Bind and send bitangents to VBO
        glBindBuffer(GL_ARRAY_BUFFER, bitangentVBO);
        glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(glm::vec3), &bitangents[0], GL_STATIC_DRAW);
        glVertexAttribPointer(11, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glEnableVertexAttribArray(11); // BiTangent - "11"

        // Unbind the VAO
        glBindVertexArray(0);
    }

    // Render function to draw the sphere
    void render() {
        // Bind the VAO
        glBindVertexArray(VAO);

        // Draw the sphere using triangles
        glDrawArrays(GL_TRIANGLE_STRIP, 0, positions.size());

        // Unbind the VAO
        glBindVertexArray(0);
    }
};

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
		: m_editorConfig{ nullptr }, m_vpCam{ nullptr }, m_shader{ nullptr }
	{
	}

	EditorApplication::~EditorApplication()
	{
		delete m_editorConfig;
		m_editorConfig = nullptr;

		delete m_vpCam;
		m_vpCam = nullptr;

		delete m_shader;
		delete m_sphere;

		delete m_albedoTexture;
		delete m_normalTexture;
	}

	int EditorApplication::RunEditor()
	{
		AssignInstance(new EditorApplication);

		return Run();
	}

	void EditorApplication::OnOpened()
	{
		Application::OnOpened();

		m_sphere = new Sphere();

		m_editorConfig->Load();

		m_vpCam = new ViewportCameraActor;
		m_vpCam->Initialise(m_editorConfig);

		Gizmos::Create(
			m_editorConfig->GetValue<int>("debug", "3d.maxLines"),
			m_editorConfig->GetValue<int>("debug", "3d.maxTris"),
			m_editorConfig->GetValue<int>("debug", "2d.maxLines"),
			m_editorConfig->GetValue<int>("debug", "2d.maxTris")
		);

		m_shader = new Shader("resources/shaders/test.shader");
		m_albedoTexture = new Texture("resources/textures/light-tree-bark_albedo.png");
		m_normalTexture = new Texture("resources/textures/light-tree-bark_normal-ogl.png");
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

		m_shader->Bind();

		m_albedoTexture->Bind(0);
		m_normalTexture->Bind(1);

		m_shader->Set("tint", vec4{ 1, 1, 1, 1 });
		m_shader->Set("albedo", 0);
		m_shader->Set("normalMap", 1);
		m_shader->Set("lightDir", vec3{ 0.5,0.707,0.5 });

		m_shader->Set("ModelMatrix", mat4(0.1f));
		m_shader->Set("ProjectionViewModel", m_vpCam->m_camera->ProjectionView());

		m_sphere->render();

		glUseProgram(0);
	}

	void EditorApplication::GenerateConfigFiles()
	{
		Application::GenerateConfigFiles();

		m_editorConfig = new Config(GetHandle());
	}
}
