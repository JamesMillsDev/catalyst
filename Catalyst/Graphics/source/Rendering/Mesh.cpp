#include "cagraphicspch.h"

#include "Rendering/Mesh.h"

#include <cstddef>
#include <iostream>
#include <numbers>
#include <vector>

#include <assimp/scene.h>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>

using std::vector;

using glm::vec3;

namespace Catalyst
{
	Mesh* Mesh::MakeQuad()
	{
		Mesh* mesh = new Mesh;

		// Define the 6 vertices for our two triangles to make a quad, 
		// in a counter-clockwise direction.
		Vertex vertices[6];
		vertices[0].position = { -.5f, 0, .5f, 1.f };
		vertices[1].position = { .5f, 0, .5f, 1.f };
		vertices[2].position = { -.5f, 0, -.5f, 1.f };

		vertices[3].position = { -0.5f, 0, -0.5f, 1.f };
		vertices[4].position = { 0.5f, 0, 0.5f, 1.f };
		vertices[5].position = { 0.5f, 0, -0.5f, 1.f };

		vertices[0].normal = { 0, 1, 0, 0 };
		vertices[1].normal = { 0, 1, 0, 0 };
		vertices[2].normal = { 0, 1, 0, 0 };
		vertices[3].normal = { 0, 1, 0, 0 };
		vertices[4].normal = { 0, 1, 0, 0 };
		vertices[5].normal = { 0, 1, 0, 0 };

		vertices[0].uvs[0] = { 0, 1 }; // Bottom Left
		vertices[1].uvs[0] = { 1, 1 }; // Bottom Right
		vertices[2].uvs[0] = { 0, 0 }; // Top Left
		vertices[3].uvs[0] = { 0, 0 }; // Top Left
		vertices[4].uvs[0] = { 1, 1 }; // Bottom Right
		vertices[5].uvs[0] = { 1, 0 }; // Top Right

		mesh->Initialise(6, vertices);
		mesh->name = "Quad";

		return mesh;
	}

	Mesh* Mesh::MakeSphere(const int _segments, const int _rings)
	{
		Mesh* mesh = new Mesh();
		vector<Vertex> vertices;
		vector<unsigned int> indices;

		for (int i = 0; i <= _rings; ++i)
		{
			float v = static_cast<float>(i) / static_cast<float>(_rings);
			float phi = v * std::numbers::pi_v<float>;

			for (int j = 0; j <= _segments; ++j)
			{
				float u = static_cast<float>(j) / static_cast<float>(_segments);
				float theta = u * 2.0f * std::numbers::pi_v<float>;
				float x = sin(phi) * cos(theta);
				float y = cos(phi);
				float z = sin(phi) * sin(theta);

				Vertex vert;
				vert.position = { x, y, z, 1.f };
				vert.normal = normalize(vec4{ x, y, z, 0.f });

				// UV coordinates (flip v for OpenGL-style mapping)
				vert.uvs[0] = { u, 1.0f - v };

				// Tangent (points in the theta direction)
				vec3 tangent = normalize(vec3(-sin(theta), 0.0f, cos(theta)));
				vert.tangent = vec4(tangent, 0.0f);

				// Bitangent (perpendicular to normal and tangent)
				vec3 biTangent = normalize(cross(vec3(vert.normal), tangent));
				vert.biTangent = vec4(biTangent, 0.0f);

				// Enable tangents
				vert.hasTangents = true;

				vertices.emplace_back(vert);
			}
		}

		for (int i = 0; i < _rings; ++i)
		{
			for (int j = 0; j < _segments; ++j)
			{
				int a = i * (_segments + 1) + j;
				int b = a + _segments + 1;
				indices.push_back(a);
				indices.push_back(b);
				indices.push_back(a + 1);
				indices.push_back(b);
				indices.push_back(b + 1);
				indices.push_back(a + 1);
			}
		}

		mesh->Initialise(static_cast<uint>(vertices.size()), vertices.data(), static_cast<uint>(indices.size()),
			indices.data());
		mesh->name = "Sphere";

		return mesh;
	}

	Mesh* Mesh::MakeCube()
	{
		Mesh* mesh = new Mesh;

		vector<Vertex> vertices =
		{
			// Front Face
			{ { -1, -1,  1, 1 }, { 0,  0,  1, 0 }, { 1,  0,  0, 0 }, { 0, 1,  0, 0 }, {}, { { 0, 0 } }, true },
			{ {  1, -1,  1, 1 }, { 0,  0,  1, 0 }, { 1,  0,  0, 0 }, { 0, 1,  0, 0 }, {}, { { 1, 0 } }, true },
			{ {  1,  1,  1, 1 }, { 0,  0,  1, 0 }, { 1,  0,  0, 0 }, { 0, 1,  0, 0 }, {}, { { 1, 1 } }, true },
			{ { -1,  1,  1, 1 }, { 0,  0,  1, 0 }, { 1,  0,  0, 0 }, { 0, 1,  0, 0 }, {}, { { 0, 1 } }, true },

			// Back Face
			{ { -1, -1, -1, 1 }, { 0,  0, -1, 0 }, { -1,  0,  0, 0 }, { 0, 1,  0, 0 }, {}, { { 1, 0 } }, true },
			{ {  1, -1, -1, 1 }, { 0,  0, -1, 0 }, { -1,  0,  0, 0 }, { 0, 1,  0, 0 }, {}, { { 0, 0 } }, true },
			{ {  1,  1, -1, 1 }, { 0,  0, -1, 0 }, { -1,  0,  0, 0 }, { 0, 1,  0, 0 }, {}, { { 0, 1 } }, true },
			{ { -1,  1, -1, 1 }, { 0,  0, -1, 0 }, { -1,  0,  0, 0 }, { 0, 1,  0, 0 }, {}, { { 1, 1 } }, true },

			// Left Face
			{ { -1, -1, -1, 1 }, { -1,  0,  0, 0 }, { 0,  0,  1, 0 }, { 0, 1,  0, 0 }, {}, { { 0, 0 } }, true },
			{ { -1,  1, -1, 1 }, { -1,  0,  0, 0 }, { 0,  0,  1, 0 }, { 0, 1,  0, 0 }, {}, { { 0, 1 } }, true },
			{ { -1,  1,  1, 1 }, { -1,  0,  0, 0 }, { 0,  0,  1, 0 }, { 0, 1,  0, 0 }, {}, { { 1, 1 } }, true },
			{ { -1, -1,  1, 1 }, { -1,  0,  0, 0 }, { 0,  0,  1, 0 }, { 0, 1,  0, 0 }, {}, { { 1, 0 } }, true },

			// Right Face
			{ { 1, -1, -1, 1 }, { 1,  0,  0, 0 }, { 0,  0, -1, 0 }, { 0, 1,  0, 0 }, {}, { { 1, 0 } }, true },
			{ { 1,  1, -1, 1 }, { 1,  0,  0, 0 }, { 0,  0, -1, 0 }, { 0, 1,  0, 0 }, {}, { { 1, 1 } }, true },
			{ { 1,  1,  1, 1 }, { 1,  0,  0, 0 }, { 0,  0, -1, 0 }, { 0, 1,  0, 0 }, {}, { { 0, 1 } }, true },
			{ { 1, -1,  1, 1 }, { 1,  0,  0, 0 }, { 0,  0, -1, 0 }, { 0, 1,  0, 0 }, {}, { { 0, 0 } }, true },

			// Top Face
			{ { -1, 1, -1, 1 }, { 0,  1,  0, 0 }, { 1,  0,  0, 0 }, { 0,  0, -1, 0 }, {}, { { 0, 1 } }, true },
			{ { 1, 1, -1, 1 }, { 0,  1,  0, 0 }, { 1,  0,  0, 0 }, { 0,  0, -1, 0 }, {}, { { 1, 1 } }, true },
			{ { 1, 1,  1, 1 }, { 0,  1,  0, 0 }, { 1,  0,  0, 0 }, { 0,  0, -1, 0 }, {}, { { 1, 0 } }, true },
			{ { -1, 1,  1, 1 }, { 0,  1,  0, 0 }, { 1,  0,  0, 0 }, { 0,  0, -1, 0 }, {}, { { 0, 0 } }, true },

			// Bottom Face
			{ { -1, -1, -1, 1 }, { 0, -1,  0, 0 }, { 1,  0,  0, 0 }, { 0,  0,  1, 0 }, {}, { { 0, 0 } }, true },
			{ { 1, -1, -1, 1 }, { 0, -1,  0, 0 }, { 1,  0,  0, 0 }, { 0,  0,  1, 0 }, {}, { { 1, 0 } }, true },
			{ { 1, -1,  1, 1 }, { 0, -1,  0, 0 }, { 1,  0,  0, 0 }, { 0,  0,  1, 0 }, {}, { { 1, 1 } }, true },
			{ { -1, -1,  1, 1 }, { 0, -1,  0, 0 }, { 1,  0,  0, 0 }, { 0,  0,  1, 0 }, {}, { { 0, 1 } }, true }
		};

		vector<unsigned int> indices =
		{
			0, 1, 2, 2, 3, 0,    // Front
			4, 5, 6, 6, 7, 4,    // Back
			8, 9, 10, 10, 11, 8, // Left
			12, 13, 14, 14, 15, 12, // Right
			16, 17, 18, 18, 19, 16, // Top
			20, 21, 22, 22, 23, 20  // Bottom
		};

		mesh->Initialise(static_cast<uint>(vertices.size()), vertices.data(), static_cast<uint>(indices.size()), indices.data());
		mesh->name = "Cube";

		return mesh;

	}

	Mesh* Mesh::MakeCylinder(int segments)
	{
		Mesh* mesh = new Mesh();
		vector<Vertex> vertices;
		vector<unsigned int> indices;

		// Generate side vertices
		for (int i = 0; i <= segments; ++i)
		{
			float u = static_cast<float>(i) / static_cast<float>(segments);
			float theta = u * 2.0f * std::numbers::pi_v<float>;
			float x = cos(theta);
			float z = sin(theta);

			vec4 normal = normalize(vec4{ x, 0, z, 0 });
			vec4 tangent = normalize(vec4{ -sin(theta), 0, cos(theta), 0 });
			vec4 biTangent = vec4{ 0, 1, 0, 0 };

			Vertex vertA;
			vertA.position = { x, 1.f, z, 1.f };
			vertA.normal = normal;
			vertA.tangent = tangent;
			vertA.biTangent = biTangent;
			vertA.uvs[0] = { u, 1.0f };
			vertA.hasTangents = true;

			Vertex vertB;
			vertB.position = { x, -1.f, z, 1.f };
			vertB.normal = normal;
			vertB.tangent = tangent;
			vertB.biTangent = biTangent;
			vertB.uvs[0] = { u, 0.0f };
			vertB.hasTangents = true;

			vertices.emplace_back(vertA);
			vertices.emplace_back(vertB);
		}

		// Generate side indices
		for (int i = 0; i < segments; ++i)
		{
			int next = (i + 1) % segments;

			indices.push_back(i * 2);
			indices.push_back(next * 2);
			indices.push_back(i * 2 + 1);

			indices.push_back(next * 2);
			indices.push_back(next * 2 + 1);
			indices.push_back(i * 2 + 1);
		}

		// Generate top and bottom cap centers
		int topCenterIndex = static_cast<int>(vertices.size());
		int bottomCenterIndex = topCenterIndex + 1;

		vertices.push_back({ { 0, 1.f, 0, 1.f }, { 0, 1, 0, 0 }, { 1, 0, 0, 0 }, { 0, 0, -1, 0 }, {}, { { 0.5f, 0.5f } }, true });
		vertices.push_back({ { 0, -1.f, 0, 1.f }, { 0, -1, 0, 0 }, { 1, 0, 0, 0 }, { 0, 0, 1, 0 }, {}, { { 0.5f, 0.5f } }, true });

		// Generate top and bottom cap vertices
		for (int i = 0; i < segments; ++i)
		{
			float theta = static_cast<float>(i) / static_cast<float>(segments) * 2.0f * std::numbers::pi_v<float>;
			float x = cos(theta);
			float z = sin(theta);
			float u = (x + 1.0f) * 0.5f;
			float v = (z + 1.0f) * 0.5f;

			// Top cap vertex
			Vertex topVert;
			topVert.position = { x, 1.f, z, 1.f };
			topVert.normal = { 0, 1, 0, 0 };
			topVert.tangent = { 1, 0, 0, 0 };
			topVert.biTangent = { 0, 0, -1, 0 };
			topVert.uvs[0] = { u, v };
			topVert.hasTangents = true;
			vertices.push_back(topVert);

			// Bottom cap vertex
			Vertex bottomVert;
			bottomVert.position = { x, -1.f, z, 1.f };
			bottomVert.normal = { 0, -1, 0, 0 };
			bottomVert.tangent = { 1, 0, 0, 0 };
			bottomVert.biTangent = { 0, 0, 1, 0 };
			bottomVert.uvs[0] = { u, v };
			bottomVert.hasTangents = true;
			vertices.push_back(bottomVert);
		}

		// Generate cap indices
		for (int i = 0; i < segments; ++i)
		{
			int next = (i + 1) % segments;
			int topVertexIndex = topCenterIndex + 2 + i;
			int bottomVertexIndex = bottomCenterIndex + 2 + i;

			// Top cap
			indices.push_back(topCenterIndex);
			indices.push_back(topVertexIndex);
			indices.push_back(topCenterIndex + 2 + next);

			// Bottom cap
			indices.push_back(bottomCenterIndex);
			indices.push_back(bottomCenterIndex + 2 + next);
			indices.push_back(bottomVertexIndex);
		}

		mesh->Initialise(static_cast<uint>(vertices.size()), vertices.data(), static_cast<uint>(indices.size()), indices.data());
		mesh->name = "Cylinder";

		return mesh;

	}

	Mesh* Mesh::MakePlane(const int _width, const int _height, const int _segments)
	{
		Mesh* mesh = new Mesh();
		vector<Vertex> vertices;
		vector<unsigned int> indices;

		float halfWidth = static_cast<float>(_width) * 0.5f;
		float halfHeight = static_cast<float>(_height) * 0.5f;

		vec4 normal = { 0.0f, 1.0f, 0.0f, 0.0f };
		vec4 tangent = { 1.0f, 0.0f, 0.0f, 0.0f }; // Tangent along X-axis
		vec4 biTangent = { 0.0f, 0.0f, -1.0f, 0.0f }; // BiTangent along Z-axis

		for (int y = 0; y <= _segments; ++y)
		{
			for (int x = 0; x <= _segments; ++x)
			{
				float u = static_cast<float>(x) / static_cast<float>(_segments);
				float v = static_cast<float>(y) / static_cast<float>(_segments);
				float px = (u * static_cast<float>(_width)) - halfWidth;
				float pz = (v * static_cast<float>(_height)) - halfHeight;

				Vertex vert;
				vert.position = { px, 0.0f, pz, 1.0f };
				vert.normal = normal;
				vert.tangent = tangent;
				vert.biTangent = biTangent;
				vert.uvs[0] = { u, v };
				vert.hasTangents = true;

				vertices.emplace_back(vert);
			}
		}

		for (int y = 0; y < _segments; ++y)
		{
			for (int x = 0; x < _segments; ++x)
			{
				int a = y * (_segments + 1) + x;
				int b = a + _segments + 1;

				indices.push_back(a);
				indices.push_back(b);
				indices.push_back(a + 1);

				indices.push_back(b);
				indices.push_back(b + 1);
				indices.push_back(a + 1);
			}
		}

		mesh->Initialise(static_cast<uint>(vertices.size()), vertices.data(), static_cast<uint>(indices.size()), indices.data());
		mesh->name = "Plane";

		return mesh;

	}

	Mesh* Mesh::FromAssimp(const aiMesh* _mesh, bool _flipTextureV)
	{
		vector<Vertex> verts;

		for (uint v = 0; v < _mesh->mNumVertices; ++v)
		{
			Vertex newVert;

			aiVector3D vert = _mesh->mVertices[v];
			newVert.position = { vert.x, vert.y, vert.z, 1 };

			if (_mesh->HasNormals())
			{
				aiVector3D norm = _mesh->mNormals[v];
				newVert.normal = { norm.x, norm.y, norm.z, 1 };
			}

			if (_mesh->HasTangentsAndBitangents())
			{
				aiVector3D tangent = _mesh->mTangents[v];
				aiVector3D biTangent = _mesh->mBitangents[v];

				newVert.tangent = { tangent.x, tangent.y, tangent.z, 1.f };
				newVert.biTangent = { biTangent.x, biTangent.y, biTangent.z, 1.f };
				newVert.hasTangents = true;
			}

			for (uint i = 0; i < _mesh->GetNumColorChannels(); ++i)
			{
				if (_mesh->HasVertexColors(i))
				{
					aiColor4D color = _mesh->mColors[i][v];
					newVert.colors[i] = { color.r, color.g, color.b, color.a };
				}
			}

			for (uint i = 0; i < _mesh->GetNumUVChannels(); ++i)
			{
				if (_mesh->HasTextureCoords(i))
				{
					aiVector3D uv = _mesh->mTextureCoords[i][v];
					newVert.uvs[i] = { uv.x, (_flipTextureV ? -uv.y : uv.y) };
				}
			}

			verts.emplace_back(newVert);
		}

		// extract indices from the first mesh
		const unsigned int numFaces = _mesh->mNumFaces;
		vector<unsigned int> indices;
		for (unsigned int i = 0; i < numFaces; i++)
		{
			indices.emplace_back(_mesh->mFaces[i].mIndices[1]);
			indices.emplace_back(_mesh->mFaces[i].mIndices[2]);
			indices.emplace_back(_mesh->mFaces[i].mIndices[0]);

			// generate a second triangle for quads
			if (_mesh->mFaces[i].mNumIndices == 4)
			{
				indices.emplace_back(_mesh->mFaces[i].mIndices[2]);
				indices.emplace_back(_mesh->mFaces[i].mIndices[3]);
				indices.emplace_back(_mesh->mFaces[i].mIndices[0]);
			}
		}

		Mesh* mesh = new Mesh;
		mesh->m_materialIndex = _mesh->mMaterialIndex;
		mesh->Initialise(static_cast<uint>(verts.size()), verts.data(), static_cast<uint>(indices.size()), indices.data());
		mesh->name = _mesh->mName.C_Str();

		return mesh;
	}

	Mesh::Mesh()
		: m_triCount{ 0 }, m_vao{ 0 }, m_vbo{ 0 }, m_ibo{ 0 }, m_materialIndex{ 0 }, name{ "none" }
	{
	}

	Mesh::Mesh(const Mesh& _other) = default;

	Mesh::~Mesh()
	{
		// If the array does not equal
		// zero, then delete the data
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ibo);
	}

	void Mesh::Render() const
	{
		glBindVertexArray(m_vao);

		// Check if we are using indices, or just vertex points
		if (m_ibo != 0)
		{
			glDrawElements(GL_TRIANGLES, 3 * static_cast<int>(m_triCount), GL_UNSIGNED_INT, nullptr);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, 3 * static_cast<int>(m_triCount));
		}
	}

	uint Mesh::GetMaterialIndex() const
	{
		return m_materialIndex;
	}

	void Mesh::Initialise(uint _vertexCount, const Vertex* _vertices, uint _indexCount, const uint* _indices)
	{
		// Check if the mesh is not initialized already
		assert(m_vao == 0);

		// Generate buffers
		glGenBuffers(1, &m_vbo);
		glGenVertexArrays(1, &m_vao);

		// Bind the vertex array, this will be our mesh buffer
		glBindVertexArray(m_vao);

		// Bind the vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		// Fill the vertex buffer
		glBufferData(GL_ARRAY_BUFFER, _vertexCount * static_cast<long long>(sizeof(Vertex)), _vertices, GL_STATIC_DRAW);

		int id = 0;

		// Enable the first element as the position 
		glEnableVertexAttribArray(id);
		glVertexAttribPointer(id++, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		                      reinterpret_cast<void*>(offsetof(Vertex, position))); // NOLINT(performance-no-int-to-ptr)

		// Enable the second element as the normal
		glEnableVertexAttribArray(id);
		glVertexAttribPointer(id++, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex),
		                      reinterpret_cast<void*>(offsetof(Vertex, normal))); // NOLINT(performance-no-int-to-ptr)

		// Enable the thirteenth-twentieth elements as the texture coordinate
		for (uint i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
		{
			glEnableVertexAttribArray(id);

			glVertexAttribPointer(id++, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			                      reinterpret_cast<void*>(offsetof(Vertex, uvs[i])));
			// NOLINT(performance-no-int-to-ptr)
		}

		if (_vertices->hasTangents)
		{
			// Enable the third element as the tangent
			glEnableVertexAttribArray(id);
			glVertexAttribPointer(id++, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex),
			                      reinterpret_cast<void*>(offsetof(Vertex, tangent)));
			// NOLINT(performance-no-int-to-ptr)

			// Enable the fourth element as the bi-tangent
			glEnableVertexAttribArray(id);
			glVertexAttribPointer(id++, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex),
			                      reinterpret_cast<void*>(offsetof(Vertex, biTangent)));
			// NOLINT(performance-no-int-to-ptr) 
		}

		// Enable the fifth-twelfth elements as the texture coordinate
		for (uint i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; ++i)
		{
			glEnableVertexAttribArray(id);
			glVertexAttribPointer(id++, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex),
			                      reinterpret_cast<void*>(offsetof(Vertex, colors[i])));
			// NOLINT(performance-no-int-to-ptr)
		}

		// Bind the indices if there are any defined
		if (_indexCount != 0)
		{
			glGenBuffers(1, &m_ibo);

			// Bind the vertex buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

			// Fill the vertex buffer
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexCount * static_cast<long long>(sizeof(unsigned int)), _indices,
			             GL_STATIC_DRAW);

			m_triCount = _indexCount / 3;
		}
		else
		{
			m_triCount = _vertexCount / 3;
		}

		// Unbind our buffers
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
