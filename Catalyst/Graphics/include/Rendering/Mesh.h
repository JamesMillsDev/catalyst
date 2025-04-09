//  ***************************************************************
//  Shader - Creation date: 26/3/2025 8:17:56 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"

#include <string>

#include <Mathematics/Color.h>

#include <assimp/mesh.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

using std::string;

using glm::vec2;
using glm::vec4;

namespace Catalyst
{
	class DLL Mesh
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

			bool hasTangents;

		};

	public:
		static Mesh* MakeQuad();
		static Mesh* MakeSphere(int _segments = 16, int _rings = 16);
		static Mesh* MakeCube();
		static Mesh* MakeCylinder(int segments = 16);
		static Mesh* MakePlane(int _width = 1, int _height = 1, int _segments = 1);
		static Mesh* FromAssimp(const aiMesh* _mesh, bool _flipTextureV = false);

	public:
		string name;

	public:
		Mesh();
		explicit Mesh(const Mesh& _other);

		~Mesh();

	public:
		void Render() const;

		[[nodiscard]] uint GetMaterialIndex() const;

	private:
		uint m_triCount;
		uint m_vao; // the Vertex Array Object
		uint m_vbo; // the Vertex Buffer Object
		uint m_ibo; // the Index Buffer Object
		uint m_materialIndex;

	private:
		void Initialise(uint _vertexCount, const Vertex* _vertices, uint _indexCount = 0, const uint* _indices = nullptr);

	};
}