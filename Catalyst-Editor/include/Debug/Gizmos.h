#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glm/mat4x4.hpp>

#include "Utility/Color.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

namespace Catalyst
{
	// a singleton class for rendering immediate-mode 3-D primitives
	class Gizmos  // NOLINT(cppcoreguidelines-special-member-functions)
	{
	public:
		static void	Create(int _maxLines, int _maxTris, int _max2DLines, int _max2DTris);
		static void	Destroy();

		// removes all Gizmos
		static void	Clear();

		// draws current Gizmo buffers, either using a combined (projection * view) matrix, or separate matrices
		static void	Draw(const mat4& _projectionView);
		static void	Draw(const mat4& _projection, const mat4& _view);

		// the projection matrix here should ideally be orthographic with a near of -1 and far of 1
		static void	Draw2D(const mat4& _projection);
		static void	Draw2D(float _screenWidth, float _screenHeight);

		/* Adds a single debug line. */
		static void	AddLine(const vec3& _v0, const vec3& _v1, const Color& _color);

		/* Adds a single debug line. */
		static void	AddLine(const vec3& _v0, const vec3& _v1, const Color& _color0, const Color& _color1);

		/* Adds a triangle. */
		static void	AddTri(const vec3& _v0, const vec3& _v1, const vec3& _v2, const Color& _color);

		/* Adds 3 unit-length lines (red,green,blue) representing the 3 axis of a transform, at the transform's translation. Optional scale available. */
		static void	AddTransform(const mat4& _transform, float _scale = 1.f);

		/* Adds a wire frame Axis-Aligned Bounding-Box with optional transform for rotation/translation. */
		static void	AddAabb(const vec3& _center, const vec3& _extents, const Color& _color, const mat4* _transform = nullptr);

		/* Adds an Axis-Aligned Bounding-Box with optional transform for rotation. */
		static void	AddAabbFilled(const vec3& _center, const vec3& _extents, const Color& _fillColour, const mat4* _transform = nullptr);

		/* Adds a cylinder aligned to the Y-axis with optional transform for rotation. */
		static void	AddCylinderFilled(const vec3& _center, float _radius, float _fHalfLength, uint32_t _segments, const Color& _fillColour, const mat4* _transform = nullptr);

		/* Adds a double-sided hollow ring in the XZ axis with optional transform for rotation. If fillColour.w == 0 then only an outer and inner line is drawn. */
		static void	AddRing(const vec3& _center, float _innerRadius, float _outerRadius, uint32_t _segments, const Color& _fillColour, const mat4* _transform = nullptr);

		/* Adds a double-sided disk in the XZ axis with optional transform for rotation. If fillColour.w == 0 then only an outer line is drawn. */
		static void	AddDisk(const vec3& _center, float _radius, uint32_t _segments, const Color& _fillColour, const mat4* _transform = nullptr);

		/* Adds an arc, around the Y-axis. If fillColour.w == 0 then only an outer line is drawn. */
		static void	AddArc(const vec3& _center, float _rotation, float _radius, float _halfAngle, uint32_t _segments, const Color& _fillColour, const mat4* _transform = nullptr);

		/* Adds an arc, around the Y-axis, starting at the inner radius and extending to the outer radius. If fillColour.w == 0 then only an outer line is drawn. */
		static void	AddArcRing(const vec3& _center, float _rotation, float _innerRadius, float _outerRadius, float _arcHalfAngle, uint32_t _segments, const Color& _fillColour, const mat4* _transform = nullptr);

		/* Adds a Sphere at a given position, with a given number of rows, and columns, radius and a max and min long and latitude. */
		static void	AddSphere(const vec3& _center, float _radius, int _rows, int _columns, const Color& _fillColour, const mat4* _transform = nullptr, float _longMin = 0.f, float _longMax = 360.f, float _latMin = -90.f, float _latMax = 90.f);

		/* Adds a capsule with a set height and radius. */
		static void	AddCapsule(const vec3& _center, float _height, float _radius, int _rows, int _cols, const Color& _fillColour, const mat4* _rotation = nullptr);

		/* Adds a single Hermite spline curve. */
		static void	AddHermiteSpline(const vec3& _start, const vec3& _end, const vec3& _tangentStart, const vec3& _tangentEnd, uint32_t _segments, const Color& _colour);

		// 2-dimensional gizmos
		static void	Add2dLine(const vec2& _start, const vec2& _end, const Color& _colour);
		static void	Add2dLine(const vec2& _start, const vec2& _end, const Color& _colour0, const Color& _colour1);
		static void	Add2dTri(const vec2& _v0, const vec2& _v1, const vec2& _v2, const Color& _colour);
		static void	Add2dTri(const vec2& _v0, const vec2& _v1, const vec2& _v2, const Color& _colour0, const Color& _colour1, const Color& _colour2);
		static void	Add2dAabb(const vec2& _center, const vec2& _extents, const Color& _colour, const mat4* _transform = nullptr);
		static void	Add2dAabbFilled(const vec2& _center, const vec2& _extents, const Color& _colour, const mat4* _transform = nullptr);
		static void	Add2dCircle(const vec2& _center, float _radius, uint32_t _segments, const Color& _colour, const mat4* _transform = nullptr);

	private:
		struct GizmoVertex
		{
			float x, y, z, w;
			float r, g, b, a;
		};

		struct GizmoLine
		{
			GizmoVertex v0;
			GizmoVertex v1;
		};

		struct GizmoTri
		{
			GizmoVertex v0;
			GizmoVertex v1;
			GizmoVertex v2;
		};

	private:
		int m_shader;

		// line data
		int m_maxLines;
		int m_lineCount;
		GizmoLine* m_lines;

		uint32_t m_lineVao;
		uint32_t m_lineVbo;

		// triangle data
		int m_maxTris;
		int m_triCount;
		GizmoTri* m_tris;

		uint32_t m_triVao;
		uint32_t m_triVbo;

		int m_transparentTriCount;
		GizmoTri* m_transparentTris;

		uint32_t m_transparentTriVao;
		uint32_t m_transparentTriVbo;

		// 2D line data
		int m_max2DLines;
		int m_2dLineCount;
		GizmoLine* m_2dLines;

		uint32_t m_2dLineVao;
		uint32_t m_2dLineVbo;

		// 2D triangle data
		int m_max2DTris;
		int m_2dTriCount;
		GizmoTri* m_2dTris;

		uint32_t m_2dTriVao;
		uint32_t m_2dTriVbo;

		static Gizmos* m_singleton;

	private:
		Gizmos(int _maxLines, int _maxTris, int _max2DLines, int _max2DTris);
		~Gizmos();

	};
}