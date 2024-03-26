#pragma once

#include <Catalyst/Catalyst.hpp>
#include <Catalyst/Engine/Utility/Color.hpp>

#include <assimp/mesh.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

using glm::vec2;
using glm::vec4;

namespace Catalyst
{
	class Mesh
	{
	public:
		struct Vertex
		{
		public:
			vec4 position;
			vec4 normal;
			vec4 tangent;
			vec4 biTangent;

			Color colors[AI_MAX_NUMBER_OF_COLOR_SETS];
			vec2 uvs[AI_MAX_NUMBER_OF_TEXTURECOORDS];

		};

	public:
		DLL Mesh();
		DLL explicit Mesh(const Mesh& _other);

		DLL virtual ~Mesh();

	public:
		DLL virtual void Render();

		DLL void Initialise(uint32 _vertexCount, const Vertex* _vertices, uint32 _indexCount = 0, const uint32* _indices = nullptr);

		DLL void InitialiseQuad();
		DLL void InitialiseFromAssimp(const aiMesh* _mesh, bool _flipTextureV = false);

		[[nodiscard]] uint32 GetMaterialIndex() const;

	protected:
		uint32 m_triCount;
		uint32 m_vao; // the Vertex Array Object
		uint32 m_vbo; // the Vertex Buffer Object
		uint32 m_ibo; // the Index Buffer Object
		uint32 m_materialIndex;

	};
}