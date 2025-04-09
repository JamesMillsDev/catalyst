#include "camathspch.h"
#include "Mathematics/Quaternion.h"

#include "Mathematics/Vector4.h"

using glm::vec3;
using glm::vec4;

namespace Catalyst
{
	Quaternion::Quaternion()
		: m_inner{ 0.f, 0.f, 0.f, 0.f }
	{
	}

	Quaternion::Quaternion(const float _x, const float _y, const float _z, const float _w)
		: m_inner{ _x, _y, _z, _w }
	{
	}

	Quaternion::Quaternion(const quat& _other)
		: m_inner{ _other }
	{
	}

	Quaternion::Quaternion(const Vector3& _euler)
		: m_inner{ quat(static_cast<vec4>(Vector4(_euler, 0.f))) }
	{

	}

	Quaternion::Quaternion(const Quaternion& _other) = default;

	Quaternion::Quaternion(Quaternion&& _other) noexcept
		: m_inner{ _other.m_inner }
	{
		_other.m_inner = {};
	}

	Quaternion::~Quaternion() = default;

	Quaternion::operator glm::qua<float>() const
	{
		return m_inner;
	}

	bool Quaternion::operator==(const Quaternion& _rhs) const
	{
		return m_inner == _rhs.m_inner;
	}

	bool Quaternion::operator!=(const Quaternion& _rhs) const
	{
		return !(*this == _rhs);
	}

	Quaternion& Quaternion::operator=(const Quaternion& _other) = default;

	Quaternion& Quaternion::operator=(Quaternion&& _other) noexcept
	{
		if (*this == _other)
			return *this;

		m_inner = _other.m_inner;
		_other.m_inner = {};

		return *this;
	}
}
