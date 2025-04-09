#include "Debug/Gizmos.h"

#include <glad/gl.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>

namespace Catalyst
{
	Gizmos* Gizmos::m_singleton = nullptr;

	Gizmos::Gizmos(const int _maxLines, const int _maxTris, const int _max2DLines, const int _max2DTris)
		: m_maxLines{ _maxLines }, m_lineCount{ 0 }, m_lines{ new GizmoLine[_maxLines] }, m_maxTris{ _maxTris },
		m_triCount{ 0 }, m_tris{ new GizmoTri[_maxTris] }, m_transparentTriCount{ 0 },
		m_transparentTris{ new GizmoTri[_maxTris] }, m_max2DLines{ _max2DLines }, m_2dLineCount{ 0 },
		m_2dLines{ new GizmoLine[_max2DLines] }, m_max2DTris{ _max2DTris }, m_2dTriCount{ 0 },
		m_2dTris{ new GizmoTri[_max2DTris] }
	{
		// create shaders
		const char* vsSource = "#version 150\n \
					 in vec4 Location; \
					 in vec4 Colour; \
					 out vec4 vColour; \
					 uniform mat4 ProjectionView; \
					 void main() { vColour = Colour; gl_Position = ProjectionView * Location; }";

		const char* fsSource = "#version 150\n \
					 in vec4 vColour; \
                     out vec4 FragColor; \
					 void main()	{ FragColor = vColour; }";


		const uint32_t vs = glCreateShader(GL_VERTEX_SHADER);
		const uint32_t fs = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vs, 1, &vsSource, nullptr);
		glCompileShader(vs);

		glShaderSource(fs, 1, &fsSource, nullptr);
		glCompileShader(fs);

		m_shader = glCreateProgram();
		glAttachShader(m_shader, vs);
		glAttachShader(m_shader, fs);
		glBindAttribLocation(m_shader, 0, "Location");
		glBindAttribLocation(m_shader, 1, "Colour");
		glLinkProgram(m_shader);

		int success = GL_FALSE;
		glGetProgramiv(m_shader, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
		{
			int infoLogLength = 0;
			glGetProgramiv(m_shader, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* infoLog = new char[infoLogLength + 1];

			glGetProgramInfoLog(m_shader, infoLogLength, nullptr, infoLog);
			printf("Error: Failed to link Gizmo shader program!\n%s\n", infoLog);
			delete[] infoLog;
		}

		glDeleteShader(vs);
		glDeleteShader(fs);

		// create VBOs
		glGenBuffers(1, &m_lineVbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_lineVbo);
		glBufferData(GL_ARRAY_BUFFER, m_maxLines * sizeof(GizmoLine), m_lines, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &m_triVbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_triVbo);
		glBufferData(GL_ARRAY_BUFFER, m_maxTris * sizeof(GizmoTri), m_tris, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &m_transparentTriVbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_transparentTriVbo);
		glBufferData(GL_ARRAY_BUFFER, m_maxTris * sizeof(GizmoTri), m_transparentTris, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &m_2dLineVbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_2dLineVbo);
		glBufferData(GL_ARRAY_BUFFER, m_max2DLines * sizeof(GizmoLine), m_2dLines, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &m_2dTriVbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_2dTriVbo);
		glBufferData(GL_ARRAY_BUFFER, m_max2DTris * sizeof(GizmoTri), m_2dTris, GL_DYNAMIC_DRAW);

		glGenVertexArrays(1, &m_lineVao);
		glBindVertexArray(m_lineVao);
		glBindBuffer(GL_ARRAY_BUFFER, m_lineVbo);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GizmoVertex), nullptr);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GizmoVertex), reinterpret_cast<void*>(16));

		glGenVertexArrays(1, &m_triVao);
		glBindVertexArray(m_triVao);
		glBindBuffer(GL_ARRAY_BUFFER, m_triVbo);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GizmoVertex), nullptr);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GizmoVertex), reinterpret_cast<void*>(16));

		glGenVertexArrays(1, &m_transparentTriVao);
		glBindVertexArray(m_transparentTriVao);
		glBindBuffer(GL_ARRAY_BUFFER, m_transparentTriVbo);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GizmoVertex), nullptr);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GizmoVertex), reinterpret_cast<void*>(16));

		glGenVertexArrays(1, &m_2dLineVao);
		glBindVertexArray(m_2dLineVao);
		glBindBuffer(GL_ARRAY_BUFFER, m_2dLineVbo);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GizmoVertex), nullptr);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GizmoVertex), reinterpret_cast<void*>(16));

		glGenVertexArrays(1, &m_2dTriVao);
		glBindVertexArray(m_2dTriVao);
		glBindBuffer(GL_ARRAY_BUFFER, m_2dTriVbo);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GizmoVertex), nullptr);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GizmoVertex), reinterpret_cast<void*>(16));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	Gizmos::~Gizmos()
	{
		delete[] m_lines;
		delete[] m_tris;
		delete[] m_transparentTris;
		glDeleteBuffers(1, &m_lineVbo);
		glDeleteBuffers(1, &m_triVbo);
		glDeleteBuffers(1, &m_transparentTriVbo);
		glDeleteVertexArrays(1, &m_lineVao);
		glDeleteVertexArrays(1, &m_triVao);
		glDeleteVertexArrays(1, &m_transparentTriVao);

		delete[] m_2dLines;
		delete[] m_2dTris;
		glDeleteBuffers(1, &m_2dLineVbo);
		glDeleteBuffers(1, &m_2dTriVbo);
		glDeleteVertexArrays(1, &m_2dLineVao);
		glDeleteVertexArrays(1, &m_2dTriVao);
		glDeleteProgram(m_shader);
	}

	void Gizmos::Create(const int _maxLines, const int _maxTris, const int _max2DLines, const int _max2DTris)
	{
		if (m_singleton == nullptr)
			m_singleton = new Gizmos(_maxLines, _maxTris, _max2DLines, _max2DTris);
	}

	void Gizmos::Destroy()
	{
		delete m_singleton;
		m_singleton = nullptr;
	}

	void Gizmos::Clear()
	{
		m_singleton->m_lineCount = 0;
		m_singleton->m_triCount = 0;
		m_singleton->m_transparentTriCount = 0;
		m_singleton->m_2dLineCount = 0;
		m_singleton->m_2dTriCount = 0;
	}

	void Gizmos::AddTransform(const mat4& _transform, const float _scale)
	{
		const vec4 vXAxis = _transform[3] + _transform[0] * _scale;
		const vec4 vYAxis = _transform[3] + _transform[1] * _scale;
		const vec4 vZAxis = _transform[3] + _transform[2] * _scale;

		const Color vRed(1, 0, 0, 1);
		const Color vGreen(0, 1, 0, 1);
		const Color vBlue(0, 0, 1, 1);

		AddLine(vec3(_transform[3]), vec3(vXAxis), vRed, vRed);
		AddLine(vec3(_transform[3]), vec3(vYAxis), vGreen, vGreen);
		AddLine(vec3(_transform[3]), vec3(vZAxis), vBlue, vBlue);
	}

	void Gizmos::AddAabb(const vec3& _center, const vec3& _extents, const Color& _color,
		const mat4* _transform)
	{
		vec3 vVerts[8];
		vec3 c = _center;
		vec3 vX(_extents.x, 0, 0);
		vec3 vY(0, _extents.y, 0);
		vec3 vZ(0, 0, _extents.z);

		if (_transform != nullptr)
		{
			vX = vec3((*_transform * vec4(vX, 0)));
			vY = vec3((*_transform * vec4(vY, 0)));
			vZ = vec3((*_transform * vec4(vZ, 0)));
			c = vec3((*_transform)[3]) + c;
		}

		// top verts
		vVerts[0] = c - vX - vZ - vY;
		vVerts[1] = c - vX + vZ - vY;
		vVerts[2] = c + vX + vZ - vY;
		vVerts[3] = c + vX - vZ - vY;

		// bottom verts
		vVerts[4] = c - vX - vZ + vY;
		vVerts[5] = c - vX + vZ + vY;
		vVerts[6] = c + vX + vZ + vY;
		vVerts[7] = c + vX - vZ + vY;

		AddLine(vVerts[0], vVerts[1], _color, _color);
		AddLine(vVerts[1], vVerts[2], _color, _color);
		AddLine(vVerts[2], vVerts[3], _color, _color);
		AddLine(vVerts[3], vVerts[0], _color, _color);

		AddLine(vVerts[4], vVerts[5], _color, _color);
		AddLine(vVerts[5], vVerts[6], _color, _color);
		AddLine(vVerts[6], vVerts[7], _color, _color);
		AddLine(vVerts[7], vVerts[4], _color, _color);

		AddLine(vVerts[0], vVerts[4], _color, _color);
		AddLine(vVerts[1], vVerts[5], _color, _color);
		AddLine(vVerts[2], vVerts[6], _color, _color);
		AddLine(vVerts[3], vVerts[7], _color, _color);
	}

	void Gizmos::AddAabbFilled(const vec3& _center, const vec3& _rvExtents, const Color& _fillColour,
		const mat4* _transform)
	{
		vec3 vVerts[8];
		vec3 tempCenter = _center;
		vec3 vX(_rvExtents.x, 0, 0);
		vec3 vY(0, _rvExtents.y, 0);
		vec3 vZ(0, 0, _rvExtents.z);

		if (_transform != nullptr)
		{
			vX = vec3((*_transform * vec4(vX, 0)));
			vY = vec3((*_transform * vec4(vY, 0)));
			vZ = vec3((*_transform * vec4(vZ, 0)));
			tempCenter = vec3((*_transform)[3]) + tempCenter;
		}

		// top verts
		vVerts[0] = tempCenter - vX - vZ - vY;
		vVerts[1] = tempCenter - vX + vZ - vY;
		vVerts[2] = tempCenter + vX + vZ - vY;
		vVerts[3] = tempCenter + vX - vZ - vY;

		// bottom verts
		vVerts[4] = tempCenter - vX - vZ + vY;
		vVerts[5] = tempCenter - vX + vZ + vY;
		vVerts[6] = tempCenter + vX + vZ + vY;
		vVerts[7] = tempCenter + vX - vZ + vY;

		const Color vWhite(1, 1, 1, 1);

		AddLine(vVerts[0], vVerts[1], vWhite, vWhite);
		AddLine(vVerts[1], vVerts[2], vWhite, vWhite);
		AddLine(vVerts[2], vVerts[3], vWhite, vWhite);
		AddLine(vVerts[3], vVerts[0], vWhite, vWhite);

		AddLine(vVerts[4], vVerts[5], vWhite, vWhite);
		AddLine(vVerts[5], vVerts[6], vWhite, vWhite);
		AddLine(vVerts[6], vVerts[7], vWhite, vWhite);
		AddLine(vVerts[7], vVerts[4], vWhite, vWhite);

		AddLine(vVerts[0], vVerts[4], vWhite, vWhite);
		AddLine(vVerts[1], vVerts[5], vWhite, vWhite);
		AddLine(vVerts[2], vVerts[6], vWhite, vWhite);
		AddLine(vVerts[3], vVerts[7], vWhite, vWhite);

		// top
		AddTri(vVerts[2], vVerts[1], vVerts[0], _fillColour);
		AddTri(vVerts[3], vVerts[2], vVerts[0], _fillColour);

		// bottom
		AddTri(vVerts[5], vVerts[6], vVerts[4], _fillColour);
		AddTri(vVerts[6], vVerts[7], vVerts[4], _fillColour);

		// front
		AddTri(vVerts[4], vVerts[3], vVerts[0], _fillColour);
		AddTri(vVerts[7], vVerts[3], vVerts[4], _fillColour);

		// back
		AddTri(vVerts[1], vVerts[2], vVerts[5], _fillColour);
		AddTri(vVerts[2], vVerts[6], vVerts[5], _fillColour);

		// left
		AddTri(vVerts[0], vVerts[1], vVerts[4], _fillColour);
		AddTri(vVerts[1], vVerts[5], vVerts[4], _fillColour);

		// right
		AddTri(vVerts[2], vVerts[3], vVerts[7], _fillColour);
		AddTri(vVerts[6], vVerts[2], vVerts[7], _fillColour);
	}

	void Gizmos::AddCylinderFilled(const vec3& _center, float _radius, float _fHalfLength, uint32_t _segments,
		const Color& _fillColour, const mat4* _transform)
	{
		Color white(1, 1, 1, 1);

		vec3 tempCenter = _transform != nullptr ? vec3((*_transform)[3]) + _center : _center;

		float segmentSize = 2 * glm::pi<float>() / static_cast<float>(_segments);

		for (uint32_t i = 0; i < _segments; ++i)
		{
			float fI = static_cast<float>(i);

			vec3 v0Top(0, _fHalfLength, 0);
			vec3 v1Top(sinf(fI * segmentSize) * _radius, _fHalfLength, cosf(fI * segmentSize) * _radius);
			vec3 v2Top(sinf((fI + 1) * segmentSize) * _radius, _fHalfLength, cosf((fI + 1) * segmentSize) * _radius);
			vec3 v0Bottom(0, -_fHalfLength, 0);
			vec3 v1Bottom(sinf(fI * segmentSize) * _radius, -_fHalfLength, cosf(fI * segmentSize) * _radius);
			vec3 v2Bottom(sinf((fI + 1) * segmentSize) * _radius, -_fHalfLength, cosf((fI + 1) * segmentSize) * _radius);

			if (_transform != nullptr)
			{
				v0Top = vec3(*_transform * vec4(v0Top, 0));
				v1Top = vec3(*_transform * vec4(v1Top, 0));
				v2Top = vec3(*_transform * vec4(v2Top, 0));
				v0Bottom = vec3(*_transform * vec4(v0Bottom, 0));
				v1Bottom = vec3(*_transform * vec4(v1Bottom, 0));
				v2Bottom = vec3(*_transform * vec4(v2Bottom, 0));
			}

			// triangles
			AddTri(tempCenter + v0Top, tempCenter + v1Top, tempCenter + v2Top, _fillColour);
			AddTri(tempCenter + v0Bottom, tempCenter + v2Bottom, tempCenter + v1Bottom, _fillColour);
			AddTri(tempCenter + v2Top, tempCenter + v1Top, tempCenter + v1Bottom, _fillColour);
			AddTri(tempCenter + v1Bottom, tempCenter + v2Bottom, tempCenter + v2Top, _fillColour);

			// lines
			AddLine(tempCenter + v1Top, tempCenter + v2Top, white, white);
			AddLine(tempCenter + v1Top, tempCenter + v1Bottom, white, white);
			AddLine(tempCenter + v1Bottom, tempCenter + v2Bottom, white, white);
		}
	}

	void Gizmos::AddRing(const vec3& _center, float _innerRadius, float _outerRadius,
		uint32_t _segments, const Color& _fillColour, const mat4* _transform)
	{
		Color vSolid = _fillColour;
		vSolid.a = 1;

		vec3 tempCenter = _transform != nullptr ? vec3((*_transform)[3]) + _center : _center;

		float fSegmentSize = 2 * glm::pi<float>() / static_cast<float>(_segments);

		for (uint32_t i = 0; i < _segments; ++i)
		{
			const float fI = static_cast<float>(i);

			vec3 v1Outer(sinf(fI * fSegmentSize) * _outerRadius, 0, cosf(fI * fSegmentSize) * _outerRadius);
			vec3 v2Outer(sinf((fI + 1) * fSegmentSize) * _outerRadius, 0, cosf((fI + 1) * fSegmentSize) * _outerRadius);
			vec3 v1Inner(sinf(fI * fSegmentSize) * _innerRadius, 0, cosf(fI * fSegmentSize) * _innerRadius);
			vec3 v2Inner(sinf((fI + 1) * fSegmentSize) * _innerRadius, 0, cosf((fI + 1) * fSegmentSize) * _innerRadius);

			if (_transform != nullptr)
			{
				v1Outer = vec3(*_transform * vec4(v1Outer, 0));
				v2Outer = vec3(*_transform * vec4(v2Outer, 0));
				v1Inner = vec3(*_transform * vec4(v1Inner, 0));
				v2Inner = vec3(*_transform * vec4(v2Inner, 0));
			}

			if (_fillColour.a > 0)
			{
				AddTri(tempCenter + v2Outer, tempCenter + v1Outer, tempCenter + v1Inner, _fillColour);
				AddTri(tempCenter + v1Inner, tempCenter + v2Inner, tempCenter + v2Outer, _fillColour);

				AddTri(tempCenter + v1Inner, tempCenter + v1Outer, tempCenter + v2Outer, _fillColour);
				AddTri(tempCenter + v2Outer, tempCenter + v2Inner, tempCenter + v1Inner, _fillColour);
			}
			else
			{
				// line
				AddLine(tempCenter + v1Inner, tempCenter + v2Inner, vSolid, vSolid);
				AddLine(tempCenter + v1Outer, tempCenter + v2Outer, vSolid, vSolid);
			}
		}
	}

	void Gizmos::AddDisk(const vec3& _center, const float _radius, const uint32_t _segments,
		const Color& _fillColour, const mat4* _transform)
	{
		Color vSolid = _fillColour;
		vSolid.a = 1;

		const vec3 tempCenter = _transform != nullptr ? vec3((*_transform)[3]) + _center : _center;

		const float fSegmentSize = 2 * glm::pi<float>() / static_cast<float>(_segments);

		for (unsigned int i = 0; i < _segments; ++i)
		{
			const float fI = static_cast<float>(i);

			vec3 v1Outer(sinf(fI * fSegmentSize) * _radius, 0, cosf(fI * fSegmentSize) * _radius);
			vec3 v2Outer(sinf((fI + 1) * fSegmentSize) * _radius, 0, cosf((fI + 1) * fSegmentSize) * _radius);

			if (_transform != nullptr)
			{
				v1Outer = vec3((*_transform * vec4(v1Outer, 0)));
				v2Outer = vec3((*_transform * vec4(v2Outer, 0)));
			}

			if (_fillColour.a > 0)
			{
				AddTri(tempCenter, tempCenter + v1Outer, tempCenter + v2Outer, _fillColour);
				AddTri(tempCenter + v2Outer, tempCenter + v1Outer, tempCenter, _fillColour);
			}
			else
			{
				// line
				AddLine(tempCenter + v1Outer, tempCenter + v2Outer, vSolid, vSolid);
			}
		}
	}

	void Gizmos::AddArc(const vec3& _center, float _rotation, float _radius, float _halfAngle, uint32_t _segments, const Color& _fillColour, const mat4* _transform)
	{
		Color vSolid = _fillColour;
		vSolid.a = 1;

		vec3 tempCenter = _transform != nullptr ? vec3((*_transform)[3]) + _center : _center;

		float fSegmentSize = 2 * _halfAngle / static_cast<float>(_segments);

		for (unsigned int i = 0; i < _segments; ++i)
		{
			const float fI = static_cast<float>(i);

			vec3 v1Outer(sinf(fI * fSegmentSize - _halfAngle + _rotation) * _radius, 0, cosf(fI * fSegmentSize - _halfAngle + _rotation) * _radius);
			vec3 v2Outer(sinf((fI + 1) * fSegmentSize - _halfAngle + _rotation) * _radius, 0, cosf((fI + 1) * fSegmentSize - _halfAngle + _rotation) * _radius);

			if (_transform != nullptr)
			{
				v1Outer = vec3((*_transform * vec4(v1Outer, 0)));
				v2Outer = vec3((*_transform * vec4(v2Outer, 0)));
			}

			if (_fillColour.a > 0)
			{
				AddTri(tempCenter, tempCenter + v1Outer, tempCenter + v2Outer, _fillColour);
				AddTri(tempCenter + v2Outer, tempCenter + v1Outer, tempCenter, _fillColour);
			}
			else
			{
				// line
				AddLine(tempCenter + v1Outer, tempCenter + v2Outer, vSolid, vSolid);
			}
		}

		// edge lines
		if (fabsf(_fillColour.a) < FLT_EPSILON)
		{
			vec3 v1Outer(sinf(-_halfAngle + _rotation) * _radius, 0, cosf(-_halfAngle + _rotation) * _radius);
			vec3 v2Outer(sinf(_halfAngle + _rotation) * _radius, 0, cosf(_halfAngle + _rotation) * _radius);

			if (_transform != nullptr)
			{
				v1Outer = vec3((*_transform * vec4(v1Outer, 0)));
				v2Outer = vec3((*_transform * vec4(v2Outer, 0)));
			}

			AddLine(tempCenter, tempCenter + v1Outer, vSolid, vSolid);
			AddLine(tempCenter, tempCenter + v2Outer, vSolid, vSolid);
		}
	}

	void Gizmos::AddArcRing(const vec3& _center, float _rotation,
		float _innerRadius, float _outerRadius, float _arcHalfAngle,
		unsigned int _segments, const Color& _fillColour, const mat4* _transform) {

		Color vSolid = _fillColour;
		vSolid.a = 1;

		vec3 tempCenter = _transform != nullptr ? vec3((*_transform)[3]) + _center : _center;

		float fSegmentSize = 2 * _arcHalfAngle / static_cast<float>(_segments);

		for (unsigned int i = 0; i < _segments; ++i)
		{
			const float fI = static_cast<float>(i);

			vec3 v1Outer(sinf(fI * fSegmentSize - _arcHalfAngle + _rotation) * _outerRadius, 0, cosf(fI * fSegmentSize - _arcHalfAngle + _rotation) * _outerRadius);
			vec3 v2Outer(sinf((fI + 1) * fSegmentSize - _arcHalfAngle + _rotation) * _outerRadius, 0, cosf((fI + 1) * fSegmentSize - _arcHalfAngle + _rotation) * _outerRadius);
			vec3 v1Inner(sinf(fI * fSegmentSize - _arcHalfAngle + _rotation) * _innerRadius, 0, cosf(fI * fSegmentSize - _arcHalfAngle + _rotation) * _innerRadius);
			vec3 v2Inner(sinf((fI + 1) * fSegmentSize - _arcHalfAngle + _rotation) * _innerRadius, 0, cosf((fI + 1) * fSegmentSize - _arcHalfAngle + _rotation) * _innerRadius);

			if (_transform != nullptr)
			{
				v1Outer = vec3((*_transform * vec4(v1Outer, 0)));
				v2Outer = vec3((*_transform * vec4(v2Outer, 0)));
				v1Inner = vec3((*_transform * vec4(v1Inner, 0)));
				v2Inner = vec3((*_transform * vec4(v2Inner, 0)));
			}

			if (_fillColour.a > 0)
			{
				AddTri(tempCenter + v2Outer, tempCenter + v1Outer, tempCenter + v1Inner, _fillColour);
				AddTri(tempCenter + v1Inner, tempCenter + v2Inner, tempCenter + v2Outer, _fillColour);

				AddTri(tempCenter + v1Inner, tempCenter + v1Outer, tempCenter + v2Outer, _fillColour);
				AddTri(tempCenter + v2Outer, tempCenter + v2Inner, tempCenter + v1Inner, _fillColour);
			}
			else
			{
				// line
				AddLine(tempCenter + v1Inner, tempCenter + v2Inner, vSolid, vSolid);
				AddLine(tempCenter + v1Outer, tempCenter + v2Outer, vSolid, vSolid);
			}
		}

		// edge lines
		if (fabsf(_fillColour.a) < FLT_EPSILON)
		{
			vec3 v1Outer(sinf(-_arcHalfAngle + _rotation) * _outerRadius, 0, cosf(-_arcHalfAngle + _rotation) * _outerRadius);
			vec3 v2Outer(sinf(_arcHalfAngle + _rotation) * _outerRadius, 0, cosf(_arcHalfAngle + _rotation) * _outerRadius);
			vec3 v1Inner(sinf(-_arcHalfAngle + _rotation) * _innerRadius, 0, cosf(-_arcHalfAngle + _rotation) * _innerRadius);
			vec3 v2Inner(sinf(_arcHalfAngle + _rotation) * _innerRadius, 0, cosf(_arcHalfAngle + _rotation) * _innerRadius);

			if (_transform != nullptr)
			{
				v1Outer = vec3((*_transform * vec4(v1Outer, 0)));
				v2Outer = vec3((*_transform * vec4(v2Outer, 0)));
				v1Inner = vec3((*_transform * vec4(v1Inner, 0)));
				v2Inner = vec3((*_transform * vec4(v2Inner, 0)));
			}

			AddLine(tempCenter + v1Inner, tempCenter + v1Outer, vSolid, vSolid);
			AddLine(tempCenter + v2Inner, tempCenter + v2Outer, vSolid, vSolid);
		}
	}

	void Gizmos::AddSphere(const vec3& _center, const float _radius, const int _rows, const int _columns,
		const Color& _fillColour, const mat4* _transform, const float _longMin, const float _longMax,
		const float _latMin, const float _latMax)
	{
		const float inverseRadius = 1 / _radius;

		// invert these first as the multiply is slightly quicker
		const float invColumns = 1.0f / static_cast<float>(_columns);
		const float invRows = 1.0f / static_cast<float>(_rows);

		constexpr float deg2Rad = glm::pi<float>() / 180;

		const vec3 tempCenter = _transform != nullptr ? vec3((*_transform)[3]) + _center : _center;

		//Lets put everything in radians first
		const float latitudinalRange = (_latMax - _latMin) * deg2Rad;
		const float longitudinalRange = (_longMax - _longMin) * deg2Rad;

		// for each row of the mesh
		vec3* v4Array = new vec3[_rows * _columns + _columns];

		for (int row = 0; row <= _rows; ++row)
		{
			// y ordinates this may be a little confusing but here we are navigating around the xAxis in GL
			const float ratioAroundXAxis = static_cast<float>(row) * invRows;
			const float radiansAboutXAxis = ratioAroundXAxis * latitudinalRange + _latMin * deg2Rad;
			const float y = _radius * sin(radiansAboutXAxis);
			const float z = _radius * cos(radiansAboutXAxis);

			for (int col = 0; col <= _columns; ++col)
			{
				const float ratioAroundYAxis = static_cast<float>(col) * invColumns;
				const float theta = ratioAroundYAxis * longitudinalRange + _longMin * deg2Rad;
				vec3 v4Point(-z * sinf(theta), y, -z * cosf(theta));
				vec3 v4Normal(inverseRadius * v4Point.x, inverseRadius * v4Point.y, inverseRadius * v4Point.z);

				if (_transform != nullptr)
				{
					v4Point = vec3(*_transform * vec4(v4Point, 0));
					v4Normal = vec3(*_transform * vec4(v4Normal, 0));
				}

				const int index = row * _columns + col % _columns;
				v4Array[index] = v4Point;
			}
		}

		for (int face = 0; face < _rows * _columns; ++face)
		{
			int iNextFace = face + 1;

			if (iNextFace % _columns == 0)
				iNextFace = iNextFace - _columns;

			AddLine(tempCenter + v4Array[face], tempCenter + v4Array[face + _columns], Color(1.f, 1.f, 1.f, 1.f), Color(1.f, 1.f, 1.f, 1.f));

			if (face % _columns == 0 && longitudinalRange < glm::pi<float>() * 2)
				continue;

			AddLine(tempCenter + v4Array[iNextFace + _columns], tempCenter + v4Array[face + _columns], Color(1.f, 1.f, 1.f, 1.f), Color(1.f, 1.f, 1.f, 1.f));

			AddTri(tempCenter + v4Array[iNextFace + _columns], tempCenter + v4Array[face], tempCenter + v4Array[iNextFace], _fillColour);
			AddTri(tempCenter + v4Array[iNextFace + _columns], tempCenter + v4Array[face + _columns], tempCenter + v4Array[face], _fillColour);
		}

		delete[] v4Array;
	}

	void Gizmos::AddCapsule(const vec3& _center, float _height, float _radius,
		int _rows, int _cols, const Color& _fillColour, const mat4* _rotation)
	{
		float sphereCenters = _height * 0.5f - _radius;
		vec4 top = vec4(0, sphereCenters, 0, 0);
		vec4 bottom = vec4(0, -sphereCenters, 0, 0);
		Color white(1.f, 1.f, 1.f, 1.f);

		if (_rotation)
		{
			top = *_rotation * top + (*_rotation)[3];
			bottom = (*_rotation) * bottom + (*_rotation)[3];
		}

		vec3 topCenter = _center + vec3(top);
		vec3 bottomCenter = _center + vec3(bottom);

		float inverseRadius = 1 / _radius;

		// invert these first as the multiply is slightly quicker
		float invColumns = 1.0f / static_cast<float>(_cols);
		float invRows = 1.0f / static_cast<float>(_rows);

		float deg2Rad = glm::pi<float>() / 180;
		float latMin = -90, latMax = 90, longMin = 0.f, longMax = 360;

		// lets put everything in radians first
		float latitudinalRange = (latMax - latMin) * deg2Rad;
		float longitudinalRange = (longMax - longMin) * deg2Rad;

		// for each row of the mesh
		vec3* v4Array = new vec3[_rows * _cols + _cols];

		for (int row = 0; row <= _rows; ++row)
		{
			// y ordinates this may be a little confusing but here we are navigating around the xAxis in GL
			float ratioAroundXAxis = static_cast<float>(row) * invRows;
			float radiansAboutXAxis = ratioAroundXAxis * latitudinalRange + (latMin * deg2Rad);
			float y = _radius * sin(radiansAboutXAxis);
			float z = _radius * cos(radiansAboutXAxis);

			for (int col = 0; col <= _cols; ++col)
			{
				float ratioAroundYAxis = static_cast<float>(col) * invColumns;
				float theta = ratioAroundYAxis * longitudinalRange + longMin * deg2Rad;
				vec3 v4Point(-z * sinf(theta), y, -z * cosf(theta));
				vec3 v4Normal(inverseRadius * v4Point.x, inverseRadius * v4Point.y, inverseRadius * v4Point.z);

				if (_rotation != nullptr)
				{
					v4Point = vec3(*_rotation * vec4(v4Point, 0));
					v4Normal = vec3(*_rotation * vec4(v4Normal, 0));
				}

				int index = row * _cols + (col % _cols);
				v4Array[index] = v4Point;
			}
		}

		// create a sphere that is split in two
		for (int face = 0; face < _rows * _cols; ++face)
		{
			int iNextFace = face + 1;

			if (iNextFace % _cols == 0)
				iNextFace = iNextFace - _cols;

			vec3 tempCenter = topCenter;
			if (face < _rows / 2 * _cols)
				tempCenter = bottomCenter;

			AddLine(tempCenter + v4Array[face], tempCenter + v4Array[face + _cols], white, white);

			if (face % _cols == 0 && longitudinalRange < (glm::pi<float>() * 2))
				continue;

			AddLine(tempCenter + v4Array[iNextFace + _cols], tempCenter + v4Array[face + _cols], white, white);
			AddTri(tempCenter + v4Array[iNextFace + _cols], tempCenter + v4Array[face], tempCenter + v4Array[iNextFace], _fillColour);
			AddTri(tempCenter + v4Array[iNextFace + _cols], tempCenter + v4Array[face + _cols], tempCenter + v4Array[face], _fillColour);
		}

		delete[] v4Array;

		for (int i = 0; i < _cols; ++i)
		{
			float x = static_cast<float>(i) / static_cast<float>(_cols);
			float x1 = static_cast<float>(i + 1) / static_cast<float>(_cols);
			x *= 2.0f * glm::pi<float>();
			x1 *= 2.0f * glm::pi<float>();

			vec3 pos = vec3(cosf(x), 0, sinf(x)) * _radius;
			vec3 pos1 = vec3(cosf(x1), 0, sinf(x1)) * _radius;

			if (_rotation)
			{
				pos = vec3(*_rotation * vec4(pos, 0));
				pos1 = vec3(*_rotation * vec4(pos1, 0));
			}

			AddTri(topCenter + pos1, bottomCenter + pos1, bottomCenter + pos, _fillColour);
			AddTri(topCenter + pos1, bottomCenter + pos, topCenter + pos, _fillColour);

			AddLine(topCenter + pos, topCenter + pos1, white, white);
			AddLine(bottomCenter + pos, bottomCenter + pos1, white, white);
			AddLine(topCenter + pos, bottomCenter + pos, white, white);
		}
	}

	void Gizmos::AddHermiteSpline(const vec3& _start, const vec3& _end,
		const vec3& _tangentStart, const vec3& _tangentEnd, uint32_t _segments, const Color& _colour) {

		_segments = _segments > 1 ? _segments : 1;

		vec3 prev = _start;

		for (uint32_t i = 1; i <= _segments; ++i)
		{
			const float s = static_cast<float>(i) / static_cast<float>(_segments);

			const float s2 = s * s;
			const float s3 = s2 * s;
			const float h1 = 2.0f * s3 - 3.0f * s2 + 1.0f;
			const float h2 = -2.0f * s3 + 3.0f * s2;
			const float h3 = s3 - 2.0f * s2 + s;
			const float h4 = s3 - s2;
			vec3 p = _start * h1 + _end * h2 + _tangentStart * h3 + _tangentEnd * h4;

			AddLine(prev, p, _colour, _colour);
			prev = p;
		}
	}

	void Gizmos::AddLine(const vec3& _v0, const vec3& _v1, const Color& _color)
	{
		AddLine(_v0, _v1, _color, _color);
	}

	void Gizmos::AddLine(const vec3& _v0, const vec3& _v1, const Color& _color0, const Color& _color1)
	{
		if (m_singleton != nullptr && m_singleton->m_lineCount < m_singleton->m_maxLines)
		{
			m_singleton->m_lines[m_singleton->m_lineCount].v0.x = _v0.x;
			m_singleton->m_lines[m_singleton->m_lineCount].v0.y = _v0.y;
			m_singleton->m_lines[m_singleton->m_lineCount].v0.z = _v0.z;
			m_singleton->m_lines[m_singleton->m_lineCount].v0.w = 1;
			m_singleton->m_lines[m_singleton->m_lineCount].v0.r = _color0.r;
			m_singleton->m_lines[m_singleton->m_lineCount].v0.g = _color0.g;
			m_singleton->m_lines[m_singleton->m_lineCount].v0.b = _color0.b;
			m_singleton->m_lines[m_singleton->m_lineCount].v0.a = _color0.a;

			m_singleton->m_lines[m_singleton->m_lineCount].v1.x = _v1.x;
			m_singleton->m_lines[m_singleton->m_lineCount].v1.y = _v1.y;
			m_singleton->m_lines[m_singleton->m_lineCount].v1.z = _v1.z;
			m_singleton->m_lines[m_singleton->m_lineCount].v1.w = 1;
			m_singleton->m_lines[m_singleton->m_lineCount].v1.r = _color1.r;
			m_singleton->m_lines[m_singleton->m_lineCount].v1.g = _color1.g;
			m_singleton->m_lines[m_singleton->m_lineCount].v1.b = _color1.b;
			m_singleton->m_lines[m_singleton->m_lineCount].v1.a = _color1.a;

			m_singleton->m_lineCount++;
		}
	}

	void Gizmos::AddTri(const vec3& _v0, const vec3& _v1, const vec3& _v2, const Color& _color)
	{
		if (m_singleton != nullptr)
		{
			if (_color.a > 1 - FLT_EPSILON)
			{
				if (m_singleton->m_triCount < m_singleton->m_maxTris)
				{
					m_singleton->m_tris[m_singleton->m_triCount].v0.x = _v0.x;
					m_singleton->m_tris[m_singleton->m_triCount].v0.y = _v0.y;
					m_singleton->m_tris[m_singleton->m_triCount].v0.z = _v0.z;
					m_singleton->m_tris[m_singleton->m_triCount].v0.w = 1;
					m_singleton->m_tris[m_singleton->m_triCount].v1.x = _v1.x;
					m_singleton->m_tris[m_singleton->m_triCount].v1.y = _v1.y;
					m_singleton->m_tris[m_singleton->m_triCount].v1.z = _v1.z;
					m_singleton->m_tris[m_singleton->m_triCount].v1.w = 1;
					m_singleton->m_tris[m_singleton->m_triCount].v2.x = _v2.x;
					m_singleton->m_tris[m_singleton->m_triCount].v2.y = _v2.y;
					m_singleton->m_tris[m_singleton->m_triCount].v2.z = _v2.z;
					m_singleton->m_tris[m_singleton->m_triCount].v2.w = 1;

					m_singleton->m_tris[m_singleton->m_triCount].v0.r = _color.r;
					m_singleton->m_tris[m_singleton->m_triCount].v0.g = _color.g;
					m_singleton->m_tris[m_singleton->m_triCount].v0.b = _color.b;
					m_singleton->m_tris[m_singleton->m_triCount].v0.a = _color.a;
					m_singleton->m_tris[m_singleton->m_triCount].v1.r = _color.r;
					m_singleton->m_tris[m_singleton->m_triCount].v1.g = _color.g;
					m_singleton->m_tris[m_singleton->m_triCount].v1.b = _color.b;
					m_singleton->m_tris[m_singleton->m_triCount].v1.a = _color.a;
					m_singleton->m_tris[m_singleton->m_triCount].v2.r = _color.r;
					m_singleton->m_tris[m_singleton->m_triCount].v2.g = _color.g;
					m_singleton->m_tris[m_singleton->m_triCount].v2.b = _color.b;
					m_singleton->m_tris[m_singleton->m_triCount].v2.a = _color.a;

					m_singleton->m_triCount++;
				}
			}
			else
			{
				if (m_singleton->m_transparentTriCount < m_singleton->m_maxTris)
				{
					m_singleton->m_transparentTris[m_singleton->m_transparentTriCount].v0.x = _v0.x;
					m_singleton->m_transparentTris[m_singleton->m_transparentTriCount].v0.y = _v0.y;
					m_singleton->m_transparentTris[m_singleton->m_transparentTriCount].v0.z = _v0.z;
					m_singleton->m_transparentTris[m_singleton->m_transparentTriCount].v0.w = 1;
					m_singleton->m_transparentTris[m_singleton->m_transparentTriCount].v1.x = _v1.x;
					m_singleton->m_transparentTris[m_singleton->m_transparentTriCount].v1.y = _v1.y;
					m_singleton->m_transparentTris[m_singleton->m_transparentTriCount].v1.z = _v1.z;
					m_singleton->m_transparentTris[m_singleton->m_transparentTriCount].v1.w = 1;
					m_singleton->m_transparentTris[m_singleton->m_transparentTriCount].v2.x = _v2.x;
					m_singleton->m_transparentTris[m_singleton->m_transparentTriCount].v2.y = _v2.y;
					m_singleton->m_transparentTris[m_singleton->m_transparentTriCount].v2.z = _v2.z;
					m_singleton->m_transparentTris[m_singleton->m_transparentTriCount].v2.w = 1;

					m_singleton->m_transparentTris[m_singleton->m_transparentTriCount].v0.r = _color.r;
					m_singleton->m_transparentTris[m_singleton->m_transparentTriCount].v0.g = _color.g;
					m_singleton->m_transparentTris[m_singleton->m_transparentTriCount].v0.b = _color.b;
					m_singleton->m_transparentTris[m_singleton->m_transparentTriCount].v0.a = _color.a;
					m_singleton->m_transparentTris[m_singleton->m_transparentTriCount].v1.r = _color.r;
					m_singleton->m_transparentTris[m_singleton->m_transparentTriCount].v1.g = _color.g;
					m_singleton->m_transparentTris[m_singleton->m_transparentTriCount].v1.b = _color.b;
					m_singleton->m_transparentTris[m_singleton->m_transparentTriCount].v1.a = _color.a;
					m_singleton->m_transparentTris[m_singleton->m_transparentTriCount].v2.r = _color.r;
					m_singleton->m_transparentTris[m_singleton->m_transparentTriCount].v2.g = _color.g;
					m_singleton->m_transparentTris[m_singleton->m_transparentTriCount].v2.b = _color.b;
					m_singleton->m_transparentTris[m_singleton->m_transparentTriCount].v2.a = _color.a;

					m_singleton->m_transparentTriCount++;
				}
			}
		}
	}

	void Gizmos::Add2dAabb(const vec2& _center, const vec2& _extents, const Color& _colour, const mat4* _transform)
	{
		vec2 verts[4];
		vec2 vX(_extents.x, 0);
		vec2 vY(0, _extents.y);

		if (_transform != nullptr)
		{
			vX = vec2((*_transform * vec4(vX, 0, 0)));
			vY = vec2((*_transform * vec4(vY, 0, 0)));
		}

		verts[0] = _center - vX - vY;
		verts[1] = _center + vX - vY;
		verts[2] = _center - vX + vY;
		verts[3] = _center + vX + vY;

		Add2dLine(verts[0], verts[1], _colour, _colour);
		Add2dLine(verts[1], verts[3], _colour, _colour);
		Add2dLine(verts[2], verts[3], _colour, _colour);
		Add2dLine(verts[2], verts[0], _colour, _colour);
	}

	void Gizmos::Add2dAabbFilled(const vec2& _center, const vec2& _extents, const Color& _colour,
		const mat4* _transform)
	{
		vec2 verts[4];
		vec2 vX(_extents.x, 0);
		vec2 vY(0, _extents.y);

		if (_transform != nullptr) {
			vX = vec2((*_transform * vec4(vX, 0, 0)));
			vY = vec2((*_transform * vec4(vY, 0, 0)));
		}

		verts[0] = _center - vX - vY;
		verts[1] = _center + vX - vY;
		verts[2] = _center + vX + vY;
		verts[3] = _center - vX + vY;

		Add2dTri(verts[0], verts[1], verts[2], _colour);
		Add2dTri(verts[0], verts[2], verts[3], _colour);
	}

	void Gizmos::Add2dCircle(const vec2& _center, const float _radius, const uint32_t _segments, const Color& _colour,
		const mat4* _transform)
	{
		Color solidColour = _colour;
		solidColour.a = 1;

		const float segmentSize = 2 * glm::pi<float>() / static_cast<float>(_segments);

		for (uint32_t i = 0; i < _segments; ++i)
		{
			const float fI = static_cast<float>(i);

			vec2 v1Outer(sinf(fI * segmentSize) * _radius, cosf(fI * segmentSize) * _radius);
			vec2 v2Outer(sinf((fI + 1) * segmentSize) * _radius, cosf((fI + 1) * segmentSize) * _radius);

			if (_transform != nullptr)
			{
				v1Outer = vec2(*_transform * vec4(v1Outer, 0, 0));
				v2Outer = vec2(*_transform * vec4(v2Outer, 0, 0));
			}

			if (_colour.a > 0)
			{
				Add2dTri(_center, _center + v1Outer, _center + v2Outer, _colour);
				Add2dTri(_center + v2Outer, _center + v1Outer, _center, _colour);
			}
			else
			{
				// line
				Add2dLine(_center + v1Outer, _center + v2Outer, solidColour, solidColour);
			}
		}
	}

	void Gizmos::Add2dLine(const vec2& _start, const vec2& _end, const Color& _colour)
	{
		Add2dLine(_start, _end, _colour, _colour);
	}

	void Gizmos::Add2dLine(const vec2& _start, const vec2& _end, const Color& _colour0, const Color& _colour1)
	{
		if (m_singleton != nullptr && m_singleton->m_2dLineCount < m_singleton->m_max2DLines)
		{
			m_singleton->m_2dLines[m_singleton->m_2dLineCount].v0.x = _start.x;
			m_singleton->m_2dLines[m_singleton->m_2dLineCount].v0.y = _start.y;
			m_singleton->m_2dLines[m_singleton->m_2dLineCount].v0.z = 1;
			m_singleton->m_2dLines[m_singleton->m_2dLineCount].v0.w = 1;
			m_singleton->m_2dLines[m_singleton->m_2dLineCount].v0.r = _colour0.r;
			m_singleton->m_2dLines[m_singleton->m_2dLineCount].v0.g = _colour0.g;
			m_singleton->m_2dLines[m_singleton->m_2dLineCount].v0.b = _colour0.b;
			m_singleton->m_2dLines[m_singleton->m_2dLineCount].v0.a = _colour0.a;
			m_singleton->m_2dLines[m_singleton->m_2dLineCount].v1.x = _end.x;
			m_singleton->m_2dLines[m_singleton->m_2dLineCount].v1.y = _end.y;
			m_singleton->m_2dLines[m_singleton->m_2dLineCount].v1.z = 1;
			m_singleton->m_2dLines[m_singleton->m_2dLineCount].v1.w = 1;
			m_singleton->m_2dLines[m_singleton->m_2dLineCount].v1.r = _colour1.r;
			m_singleton->m_2dLines[m_singleton->m_2dLineCount].v1.g = _colour1.g;
			m_singleton->m_2dLines[m_singleton->m_2dLineCount].v1.b = _colour1.b;
			m_singleton->m_2dLines[m_singleton->m_2dLineCount].v1.a = _colour1.a;

			m_singleton->m_2dLineCount++;
		}
	}

	void Gizmos::Add2dTri(const vec2& _v0, const vec2& _v1, const vec2& _v2, const Color& _colour)
	{
		Add2dTri(_v0, _v1, _v2, _colour, _colour, _colour);
	}

	void Gizmos::Add2dTri(const vec2& _v0, const vec2& _v1, const vec2& _v2, const Color& _colour0, const Color& _colour1, const Color& _colour2)
	{
		if (m_singleton != nullptr)
		{
			if (m_singleton->m_2dTriCount < m_singleton->m_max2DTris)
			{
				m_singleton->m_2dTris[m_singleton->m_2dTriCount].v0.x = _v0.x;
				m_singleton->m_2dTris[m_singleton->m_2dTriCount].v0.y = _v0.y;
				m_singleton->m_2dTris[m_singleton->m_2dTriCount].v0.z = 1;
				m_singleton->m_2dTris[m_singleton->m_2dTriCount].v0.w = 1;
				m_singleton->m_2dTris[m_singleton->m_2dTriCount].v1.x = _v1.x;
				m_singleton->m_2dTris[m_singleton->m_2dTriCount].v1.y = _v1.y;
				m_singleton->m_2dTris[m_singleton->m_2dTriCount].v1.z = 1;
				m_singleton->m_2dTris[m_singleton->m_2dTriCount].v1.w = 1;
				m_singleton->m_2dTris[m_singleton->m_2dTriCount].v2.x = _v2.x;
				m_singleton->m_2dTris[m_singleton->m_2dTriCount].v2.y = _v2.y;
				m_singleton->m_2dTris[m_singleton->m_2dTriCount].v2.z = 1;
				m_singleton->m_2dTris[m_singleton->m_2dTriCount].v2.w = 1;
				m_singleton->m_2dTris[m_singleton->m_2dTriCount].v0.r = _colour0.r;
				m_singleton->m_2dTris[m_singleton->m_2dTriCount].v0.g = _colour0.g;
				m_singleton->m_2dTris[m_singleton->m_2dTriCount].v0.b = _colour0.b;
				m_singleton->m_2dTris[m_singleton->m_2dTriCount].v0.a = _colour0.a;
				m_singleton->m_2dTris[m_singleton->m_2dTriCount].v1.r = _colour1.r;
				m_singleton->m_2dTris[m_singleton->m_2dTriCount].v1.g = _colour1.g;
				m_singleton->m_2dTris[m_singleton->m_2dTriCount].v1.b = _colour1.b;
				m_singleton->m_2dTris[m_singleton->m_2dTriCount].v1.a = _colour1.a;
				m_singleton->m_2dTris[m_singleton->m_2dTriCount].v2.r = _colour2.r;
				m_singleton->m_2dTris[m_singleton->m_2dTriCount].v2.g = _colour2.g;
				m_singleton->m_2dTris[m_singleton->m_2dTriCount].v2.b = _colour2.b;
				m_singleton->m_2dTris[m_singleton->m_2dTriCount].v2.a = _colour2.a;

				m_singleton->m_2dTriCount++;
			}
		}
	}

	void Gizmos::Draw(const mat4& _projection, const mat4& _view)
	{
		Draw(_projection * _view);
	}

	void Gizmos::Draw(const mat4& _projectionView)
	{
		if (m_singleton != nullptr && (m_singleton->m_lineCount > 0 || m_singleton->m_triCount > 0 || m_singleton->m_transparentTriCount > 0))
		{
			int shader = 0;
			glGetIntegerv(GL_CURRENT_PROGRAM, &shader);

			glUseProgram(m_singleton->m_shader);

			const int projectionViewUniform = glGetUniformLocation(m_singleton->m_shader, "ProjectionView");
			glUniformMatrix4fv(projectionViewUniform, 1, false, value_ptr(_projectionView));

			if (m_singleton->m_lineCount > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_singleton->m_lineVbo);
				glBufferSubData(GL_ARRAY_BUFFER, 0, m_singleton->m_lineCount * sizeof(GizmoLine), m_singleton->m_lines);

				glBindVertexArray(m_singleton->m_lineVao);
				glDrawArrays(GL_LINES, 0, m_singleton->m_lineCount * 2);
			}

			if (m_singleton->m_triCount > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_singleton->m_triVbo);
				glBufferSubData(GL_ARRAY_BUFFER, 0, m_singleton->m_triCount * sizeof(GizmoTri), m_singleton->m_tris);

				glBindVertexArray(m_singleton->m_triVao);
				glDrawArrays(GL_TRIANGLES, 0, m_singleton->m_triCount * 3);
			}

			if (m_singleton->m_transparentTriCount > 0)
			{
				// not ideal to store these, but Gizmos must work stand-alone
				const GLboolean blendEnabled = glIsEnabled(GL_BLEND);
				GLboolean depthMask = GL_TRUE;
				glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);
				int src, dst;
				glGetIntegerv(GL_BLEND_SRC, &src);
				glGetIntegerv(GL_BLEND_DST, &dst);

				// setup blend states
				if (blendEnabled == GL_FALSE)
					glEnable(GL_BLEND);

				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glDepthMask(GL_FALSE);

				glBindBuffer(GL_ARRAY_BUFFER, m_singleton->m_transparentTriVbo);
				glBufferSubData(GL_ARRAY_BUFFER, 0, m_singleton->m_transparentTriCount * sizeof(GizmoTri), m_singleton->m_transparentTris);

				glBindVertexArray(m_singleton->m_transparentTriVao);
				glDrawArrays(GL_TRIANGLES, 0, m_singleton->m_transparentTriCount * 3);

				// reset state
				glDepthMask(depthMask);
				glBlendFunc(src, dst);
				if (blendEnabled == GL_FALSE)
					glDisable(GL_BLEND);
			}

			glUseProgram(shader);
		}
	}

	void Gizmos::Draw2D(const float _screenWidth, const float _screenHeight)
	{
		Draw2D(glm::ortho(0.f, _screenWidth, 0.f, _screenHeight));
	}

	void Gizmos::Draw2D(const mat4& _projection)
	{
		if (m_singleton != nullptr && (m_singleton->m_2dLineCount > 0 || m_singleton->m_2dTriCount > 0))
		{
			int shader = 0;
			glGetIntegerv(GL_CURRENT_PROGRAM, &shader);

			glUseProgram(m_singleton->m_shader);

			const int projectionViewUniform = glGetUniformLocation(m_singleton->m_shader, "ProjectionView");
			glUniformMatrix4fv(projectionViewUniform, 1, false, value_ptr(_projection));

			if (m_singleton->m_2dLineCount > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_singleton->m_2dLineVbo);
				glBufferSubData(GL_ARRAY_BUFFER, 0, m_singleton->m_2dLineCount * sizeof(GizmoLine), m_singleton->m_2dLines);

				glBindVertexArray(m_singleton->m_2dLineVao);
				glDrawArrays(GL_LINES, 0, m_singleton->m_2dLineCount * 2);
			}

			if (m_singleton->m_2dTriCount > 0)
			{
				const GLboolean blendEnabled = glIsEnabled(GL_BLEND);

				GLboolean depthMask = GL_TRUE;
				glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);

				int src, dst;
				glGetIntegerv(GL_BLEND_SRC, &src);
				glGetIntegerv(GL_BLEND_DST, &dst);

				if (blendEnabled == GL_FALSE)
					glEnable(GL_BLEND);

				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glDepthMask(GL_FALSE);

				glBindBuffer(GL_ARRAY_BUFFER, m_singleton->m_2dTriVbo);
				glBufferSubData(GL_ARRAY_BUFFER, 0, m_singleton->m_2dTriCount * sizeof(GizmoTri), m_singleton->m_2dTris);

				glBindVertexArray(m_singleton->m_2dTriVao);
				glDrawArrays(GL_TRIANGLES, 0, m_singleton->m_2dTriCount * 3);

				glDepthMask(depthMask);

				glBlendFunc(src, dst);

				if (blendEnabled == GL_FALSE)
					glDisable(GL_BLEND);
			}

			glUseProgram(shader);
		}
	}
}