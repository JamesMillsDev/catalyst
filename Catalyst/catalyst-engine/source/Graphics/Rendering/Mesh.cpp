#include <Catalyst/Graphics/Rendering/Mesh.hpp>

#include <GL/glew.h>

#include <assimp/scene.h>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>

#include <cstddef>

using std::vector;

using glm::vec3;

namespace Catalyst
{
	Mesh::Mesh()
		: m_triCount{ 0 }, m_vao{ 0 }, m_vbo{ 0 }, m_ibo{ 0 }, m_materialIndex{ 0 }
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

	void Mesh::Render()
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

	void Mesh::Initialise(const uint32 _vertexCount, const Vertex* _vertices, const uint32 _indexCount, const uint32* _indices)
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
		glVertexAttribPointer(id++, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));// NOLINT(performance-no-int-to-ptr)

		// Enable the second element as the normal
		glEnableVertexAttribArray(id);
		glVertexAttribPointer(id++, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));// NOLINT(performance-no-int-to-ptr)

		// Enable the thirteenth-twentieth elements as the texture coordinate
		for (uint32 i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
		{
			glEnableVertexAttribArray(id);

			glVertexAttribPointer(id++, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uvs[i])));  // NOLINT(performance-no-int-to-ptr)
		}

		if (_vertices->hasTangents)
		{
			// Enable the third element as the tangent
			glEnableVertexAttribArray(id);
			glVertexAttribPointer(id++, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tangent)));// NOLINT(performance-no-int-to-ptr)

			// Enable the fourth element as the bi-tangent
			glEnableVertexAttribArray(id);
			glVertexAttribPointer(id++, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, biTangent)));// NOLINT(performance-no-int-to-ptr) 
		}

		// Enable the fifth-twelfth elements as the texture coordinate
		for (uint32 i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; ++i)
		{
			glEnableVertexAttribArray(id);
			glVertexAttribPointer(id++, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, colors[i])));  // NOLINT(performance-no-int-to-ptr)
		}

		// Bind the indices if there are any defined
		if (_indexCount != 0)
		{
			glGenBuffers(1, &m_ibo);

			// Bind the vertex buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

			// Fill the vertex buffer
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexCount * static_cast<long long>(sizeof(unsigned int)), _indices, GL_STATIC_DRAW);

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

	void Mesh::InitialiseQuad()
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

		// Define the 6 vertices for our two triangles to make a quad, 
		// in a counter-clockwise direction.
		Vertex vertices[6];
		vertices[0].position = { -.5f, 0,  .5f, 1.f };
		vertices[1].position = { .5f, 0,  .5f, 1.f };
		vertices[2].position = { -.5f, 0, -.5f, 1.f };

		vertices[3].position = { -0.5f, 0, -0.5f, 1.f };
		vertices[4].position = { 0.5f, 0,  0.5f, 1.f };
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

		// Fill the vertex buffer
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex),
			vertices, GL_STATIC_DRAW);

		// Now we will enable the first element as the position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

		// Enable the second element as the normal
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(16));

		// Enable the third element as the texture coordinate
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(32));

		// Next we unbind the buffers
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// This is a quad made up of two triangles
		m_triCount = 2;
	}

	void Mesh::InitialiseFromAssimp(const aiMesh* _mesh, const bool _flipTextureV)
	{
		vector<Vertex> verts;

		for (uint32 v = 0; v < _mesh->mNumVertices; ++v)
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

			for (uint32 i = 0; i < _mesh->GetNumColorChannels(); ++i)
			{
				if (_mesh->HasVertexColors(i))
				{
					aiColor4D color = _mesh->mColors[i][v];
					newVert.colors[i] = { color.r, color.g, color.b, color.a };
				}
			}

			for (uint32 i = 0; i < _mesh->GetNumUVChannels(); ++i)
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

		m_materialIndex = _mesh->mMaterialIndex;

		Initialise(static_cast<unsigned int>(verts.size()), verts.data(), static_cast<unsigned int>(indices.size()), indices.data());
	}

	uint32 Mesh::GetMaterialIndex() const
	{
		return m_materialIndex;
	}
}
