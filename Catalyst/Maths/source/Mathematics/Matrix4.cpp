#include "camathspch.h"
#include "Mathematics/Matrix4.h"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Catalyst
{
	Matrix4::Matrix4()
		: m_inner {
			0.f, 0.f, 0.f, 0.f,
			0.f, 0.f, 0.f, 0.f,
			0.f, 0.f, 0.f, 0.f,
			0.f, 0.f, 0.f, 0.f
		}
	{
	}

	Matrix4::Matrix4(const float _scalar)
		: m_inner {
			_scalar, 0.f, 0.f, 0.f,
			0.f, _scalar, 0.f, 0.f,
			0.f, 0.f, _scalar, 0.f,
			0.f, 0.f, 0.f, _scalar
		}
	{
	}

	Matrix4::Matrix4(const float _11, const float _21, const float _31, const float _41, 
		const float _12, const float _22, const float _32, const float _42, 
		const float _13, const float _23, const float _33, const float _43,
		const float _14, const float _24, const float _34, const float _44)
			: m_inner {
				_11, _21, _31, _41,
				_12, _22, _32, _42,
				_13, _23, _33, _43,
				_14, _24, _34, _44
			}
	{
	}

	Matrix4::Matrix4(const vec4& _x, const vec4& _y, const vec4& _z, const vec4& _w)
		: m_inner{ _x, _y, _z, _w }
	{
	}

	Matrix4::Matrix4(const mat4& _other)
		: m_inner{ _other }
	{
	}

	Matrix4::Matrix4(const Matrix4& _other) = default;

	Matrix4::Matrix4(Matrix4&& _other) noexcept
		: m_inner{ _other.m_inner }
	{
		_other.m_inner = {};
	}

	Matrix4::~Matrix4() = default;

	Matrix4 Matrix4::MakeTranslation(const Vector3& _trans)
	{
		return translate(mat4(1.f), vec3{ _trans.y, _trans.z, _trans.x });
	}

	Matrix4 Matrix4::MakeScale(const Vector3& _scale)
	{
		return scale(mat4(1.f), vec3{ _scale.y, _scale.z, _scale.x });
	}

	Matrix4 Matrix4::MakeRotation(const Quaternion& _rotation)
	{
		return toMat4(static_cast<quat>(_rotation));
	}

	Matrix4 Matrix4::MakeEuler(const Vector3& _euler)
	{
		return toMat4(static_cast<quat>(vec3{ _euler.y, _euler.z, _euler.x }));
	}

	Matrix4 Matrix4::MakeTransform(const Vector3& _trans, const Quaternion& _rot, const Vector3& _scale)
	{
		return MakeTranslation(_trans) * MakeRotation(_rot) * MakeScale(_scale);
	}

	Matrix4 Matrix4::MakeTransform(const Vector3& _trans, const Vector3& _euler, const Vector3& _scale)
	{
		return MakeTranslation(_trans) * MakeEuler(_euler) * MakeScale(_scale);
	}

	Matrix4 Matrix4::Perspective(const float _fov, const float _aspect, const float _near, const float _far)
	{
		return glm::perspective(_fov, _aspect, _near, _far);
	}

	Matrix4 Matrix4::Orthographic(const float _left, const float _right, const float _bottom, const float _top)
	{
		return glm::ortho(_left, _right, _bottom, _top);
	}

	Matrix4 Matrix4::LookAt(const Vector3& _eye, const Vector3& _target, const Vector3& _up)
	{
		return lookAt(static_cast<vec3>(_eye), static_cast<vec3>(_target), static_cast<vec3>(_up));
	}

	Matrix4 Matrix4::Identity()
	{
		return
		{
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		};
	}

	void Matrix4::LookAt(const Vector3& _target)
	{
		LookAt(_target, Up());
	}

	void Matrix4::LookAt(const Vector3& _target, const Vector3& _up)
	{
		SetRotation(
			Quaternion(lookAt(
				static_cast<vec3>(Translation()),
				static_cast<vec3>(_target),
				static_cast<vec3>(_up)
			))
		);
	}

	void Matrix4::TRS(const Vector3& _trans, const Quaternion& _rot, const Vector3& _scale)
	{
		m_inner = translate(mat4(1.f), { _trans.y, _trans.z, _trans.x }) *
			toMat4(static_cast<quat>(_rot)) *
			scale(mat4(1.f), { _scale.y, _scale.z, _scale.x });
	}

	void Matrix4::TRS(const Vector3& _trans, const Vector3& _euler, const Vector3& _scale)
	{
		TRS(_trans, Quaternion(_euler), _scale);
	}

	Matrix4 Matrix4::Inverse() const
	{
		return inverse(m_inner);
	}

	Vector3 Matrix4::Forward() const
	{
		//const mat4 inverted = inverse(m_inner);
		return normalize(vec3(m_inner[2]));
	}

	Vector3 Matrix4::Right() const
	{
		//const mat4 inverted = inverse(m_inner);
		return normalize(vec3(m_inner[0]));
	}

	Vector3 Matrix4::Up() const
	{
		//const mat4 inverted = inverse(m_inner);
		return normalize(vec3(m_inner[1]));
	}

	Vector3 Matrix4::Translation() const
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(m_inner, scale, rot, trans, skew, perspective);

		return trans;
	}

	Vector3 Matrix4::Scale() const
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(m_inner, scale, rot, trans, skew, perspective);

		return scale;
	}

	Quaternion Matrix4::Rotation() const
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(m_inner, scale, rot, trans, skew, perspective);

		return rot;
	}

	Vector3 Matrix4::Euler() const
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(m_inner, scale, rot, trans, skew, perspective);

		return degrees(eulerAngles(rot));
	}

	void Matrix4::SetTranslation(const Vector3& _trans)
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(m_inner, scale, rot, trans, skew, perspective);

		m_inner = translate(mat4(1.f), static_cast<vec3>(_trans)) * toMat4(rot) * glm::scale(mat4(1.f), scale);
	}

	void Matrix4::SetScale(const Vector3& _scale)
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(m_inner, scale, rot, trans, skew, perspective);

		m_inner = translate(mat4(1.f), trans) * toMat4(rot) * glm::scale(mat4(1.f), static_cast<vec3>(_scale));
	}

	void Matrix4::SetRotation(const Quaternion& _rot)
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(m_inner, scale, rot, trans, skew, perspective);

		m_inner = translate(mat4(1.f), trans) * toMat4(static_cast<quat>(_rot)) * glm::scale(mat4(1.f), scale);
	}

	void Matrix4::SetEuler(const Vector3& _euler)
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(m_inner, scale, rot, trans, skew, perspective);

		m_inner = translate(mat4(1.f), trans) * 
			toMat4(quat(radians(vec3{ _euler.y, _euler.z, _euler.x }))) *
			glm::scale(mat4(1.f), scale);
	}

	void Matrix4::Translate(const Vector3& _delta)
	{
		m_inner = glm::translate(m_inner, vec3{ _delta.y, _delta.z, -_delta.x });
	}

	void Matrix4::ScaleBy(const Vector3& _delta)
	{
		m_inner = glm::scale(m_inner, static_cast<vec3>(_delta));
	}

	void Matrix4::Rotate(const Quaternion& _delta)
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(m_inner, scale, rot, trans, skew, perspective);

		m_inner = translate(mat4(1.f), trans) *
			(toMat4(static_cast<quat>(_delta)) * toMat4(rot)) *
			glm::scale(mat4(1.f), scale);
	}

	void Matrix4::Rotate(const Vector3& _delta)
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(m_inner, scale, rot, trans, skew, perspective);

		m_inner = translate(mat4(1.f), trans) *
			toMat4(quat(radians(static_cast<vec3>(_delta) + eulerAngles(rot)))) *
			glm::scale(mat4(1.f), scale);
	}

	Matrix4::operator glm::mat<4, 4, float>() const
	{
		return m_inner;
	}

	bool Matrix4::operator==(const Matrix4& _rhs) const
	{
		return m_inner == _rhs.m_inner;
	}

	bool Matrix4::operator!=(const Matrix4& _rhs) const
	{
		return !(*this == _rhs);
	}

	Matrix4 Matrix4::operator*(const Matrix4& _rhs) const
	{
		return m_inner * _rhs.m_inner;
	}

	Matrix4& Matrix4::operator=(const Matrix4& _other) = default;

	Matrix4& Matrix4::operator=(Matrix4&& _other) noexcept
	{
		if (*this == _other)
			return *this;

		m_inner = _other.m_inner;
		_other.m_inner = {};

		return *this;
	}
}
