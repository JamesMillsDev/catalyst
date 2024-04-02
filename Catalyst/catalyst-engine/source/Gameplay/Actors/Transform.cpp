#include <Catalyst/Gameplay/Actors/Transform.hpp>

#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

using glm::quat;
using glm::vec4;

namespace Catalyst
{
	Transform::Transform()
		: m_parent{ nullptr }, m_transform{ mat4{ 1.f } }
	{
	}

	Transform::Transform(const mat4& _transform)
		: m_parent{ nullptr }, m_transform{ _transform }
	{
	}

	Transform::Transform(const vec3 _position, const vec3 _size, const vec3 _rotation)
		: m_parent{ nullptr }, m_transform{ mat4{ 1.f } }
	{
		TRS(_position, _rotation, _size);
	}

	Transform::Transform(Transform&& _other) noexcept
	{
		m_parent = _other.m_parent;
		m_children = _other.m_children;
		m_transform = _other.m_transform;

		_other.m_parent = nullptr;
		_other.m_children.clear();
		_other.m_transform = {};
	}

	Transform::~Transform()
	{
		m_parent = nullptr;
		m_children.clear();
		m_transform = {};
	}

	mat4 Transform::Global() const
	{
		return m_parent != nullptr ? m_parent->m_transform * m_transform : Local();
	}

	mat4 Transform::Local() const
	{
		return m_transform;
	}

	void Transform::SetParent(const Transform* _parent)
	{
		if (_parent == nullptr)
		{
			if (m_parent != nullptr)
				m_listChanges.emplace_back(&Transform::RemoveChild, m_parent, this);
		}
		else
		{
			m_listChanges.emplace_back(&Transform::AddChild, m_parent, this);
		}
	}

	Transform* Transform::Parent() const
	{
		return m_parent;
	}

	list<Transform*> Transform::Children()
	{
		return m_children;
	}

	vec3 Transform::Location() const
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(Global(), scale, rot, trans, skew, perspective);

		return trans;
	}

	vec3 Transform::LocalLocation() const
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(Local(), scale, rot, trans, skew, perspective);

		return trans;
	}

	void Transform::SetLocation(const vec3& _newLocation)
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(Local(), scale, rot, trans, skew, perspective);

		m_transform = translate(mat4(1.f), _newLocation) *
			toMat4(rot) *
			glm::scale(mat4(1.f), scale);
	}

	void Transform::UpdateLocation(const vec3& _amount)
	{
		m_transform = glm::translate(m_transform, _amount);
	}

	vec3 Transform::Scale() const
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(Global(), scale, rot, trans, skew, perspective);

		return scale;
	}

	vec3 Transform::LocalScale() const
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(Local(), scale, rot, trans, skew, perspective);

		return scale;
	}

	void Transform::SetScale(const vec3& _newScale)
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(m_transform, scale, rot, trans, skew, perspective);

		m_transform = translate(mat4(1.f), trans) *
			toMat4(rot) *
			glm::scale(mat4(1.f), _newScale);
	}

	void Transform::UpdateScale(const vec3& _amount)
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(m_transform, scale, rot, trans, skew, perspective);

		m_transform = translate(mat4(1.f), trans) *
			toMat4(rot) *
			glm::scale(mat4(1.f), scale + _amount);
	}

	vec3 Transform::Rotation() const
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(Global(), scale, rot, trans, skew, perspective);

		return glm::degrees(eulerAngles(rot));
	}

	vec3 Transform::LocalRotation() const
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(Local(), scale, rot, trans, skew, perspective);

		return glm::degrees(eulerAngles(rot));
	}

	void Transform::SetRotation(const vec3& _newRot)
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(m_transform, scale, rot, trans, skew, perspective);

		m_transform = translate(mat4(1.f), trans) *
			toMat4(quat(_newRot)) *
			glm::scale(mat4(1.f), scale);
	}

	void Transform::SetRotation(const quat& _newRot)
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(m_transform, scale, rot, trans, skew, perspective);

		m_transform = translate(mat4(1.f), trans) *
			toMat4(_newRot) *
			glm::scale(mat4(1.f), scale);
	}

	void Transform::UpdateRotation(const vec3& _amount)
	{
		m_transform = m_transform * toMat4(quat(_amount));
	}

	void Transform::TRS(const vec3& _loc, const vec3& _angle, const vec3& _scale)
	{
		SetLocation(_loc);
		SetScale(_scale);
		SetRotation(_angle);
	}

	vec3 Transform::Forward() const
	{
		vec3 rot = glm::radians(Rotation());

		return
		{
			glm::cos(rot.x) * glm::sin(rot.y),
			-glm::sin(rot.x),
			glm::cos(rot.x) * glm::cos(rot.y)
		};
	}

	vec3 Transform::Right() const
	{
		vec3 rot = glm::radians(Rotation());

		return
		{
			-glm::cos(rot.y),
			0,
			glm::sin(rot.y)
		};
	}

	vec3 Transform::Up() const
	{
		vec3 rot = glm::radians(Rotation());

		return
		{
			glm::sin(rot.x) * glm::sin(rot.y),
			glm::cos(rot.x),
			glm::sin(rot.x) * glm::cos(rot.y)
		};
	}

	void Transform::Tick(float _deltaTime)
	{
		for (auto& [fnc, parent, child] : m_listChanges)
			std::invoke(fnc, parent, child);

		m_listChanges.clear();
	}

	void Transform::AddChild(Transform* _child)
	{
		if (std::ranges::find(m_children, _child) != m_children.end())
			return;

		if (_child->m_parent != nullptr)
			_child->m_parent->RemoveChild(_child);

		_child->m_parent = this;
		m_children.emplace_back(_child);
	}

	void Transform::RemoveChild(Transform* _child)
	{
		if (std::ranges::find(m_children, _child) == m_children.end())
			return;

		if (_child->m_parent == this)
		{
			_child->m_parent = nullptr;

			const list<Transform*>::iterator iter = std::ranges::find(m_children, _child);
			if (iter != m_children.end())
				m_children.erase(iter, iter);
		}
	}
}