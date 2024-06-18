#include "cagameplaypch.h"
#include "ActorTransform.h"

#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/matrix_decompose.hpp>

using glm::vec4;

namespace Catalyst
{
	mat4 ActorTransform::LocalToWorld() const
	{
		return m_parent ? *m_transform * m_parent->LocalTransform() : LocalTransform();
	}

	mat4 ActorTransform::LocalTransform() const
	{
		return *m_transform;
	}

	void ActorTransform::SetParent(ActorTransform* _newParent)
	{
		if(_newParent)
		{
			if (m_parent)
				m_changes.emplace_back(&ActorTransform::RemoveChild, m_parent, this);
		}
		else
		{
			m_changes.emplace_back(&ActorTransform::AddChild, _newParent, this);
		}
	}

	ActorTransform* ActorTransform::Parent() const
	{
		return m_parent;
	}

	list<ActorTransform*> ActorTransform::Children() const
	{
		return m_children;
	}

	vec3 ActorTransform::Location() const
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(LocalToWorld(), scale, rot, trans, skew, perspective);

		return trans;
	}

	vec3 ActorTransform::LocalLocation() const
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(LocalTransform(), scale, rot, trans, skew, perspective);

		return trans;
	}

	void ActorTransform::SetLocation(vec3& _new) const
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(LocalTransform(), scale, rot, trans, skew, perspective);

		_new.y *= -1.f;

		*m_transform = translate(mat4(1.f), _new) *
			toMat4(rot) *
			glm::scale(mat4(1.f), scale);
	}

	void ActorTransform::UpdateLocation(vec3& _delta) const
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(LocalTransform(), scale, rot, trans, skew, perspective);

		_delta.y *= -1.f;

		*m_transform = translate(mat4(1.f), trans + _delta) *
			toMat4(rot) *
			glm::scale(mat4(1.f), scale);
	}

	vec3 ActorTransform::Scale() const
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(LocalToWorld(), scale, rot, trans, skew, perspective);

		return scale;
	}

	vec3 ActorTransform::LocalScale() const
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(LocalTransform(), scale, rot, trans, skew, perspective);

		return scale;
	}

	void ActorTransform::SetScale(const vec3& _new) const
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(LocalTransform(), scale, rot, trans, skew, perspective);

		*m_transform = translate(mat4(1.f), trans) *
			toMat4(rot) *
			glm::scale(mat4(1.f), _new);
	}

	void ActorTransform::UpdateScale(const vec3& _delta) const
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(LocalTransform(), scale, rot, trans, skew, perspective);

		*m_transform = translate(mat4(1.f), trans) *
			toMat4(rot) *
			glm::scale(mat4(1.f), scale + _delta);
	}

	quat ActorTransform::Rotation() const
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(LocalToWorld(), scale, rot, trans, skew, perspective);

		return rot;
	}

	quat ActorTransform::LocalRotation() const
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(LocalTransform(), scale, rot, trans, skew, perspective);

		return rot;
	}

	void ActorTransform::SetRotation(const quat& _new) const
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(LocalTransform(), scale, rot, trans, skew, perspective);

		*m_transform = translate(mat4(1.f), trans) *
			toMat4(_new) *
			glm::scale(mat4(1.f), scale);
	}

	vec3 ActorTransform::EulerAngles() const
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(LocalToWorld(), scale, rot, trans, skew, perspective);

		return glm::degrees(eulerAngles(rot));
	}

	vec3 ActorTransform::LocalEulerAngles() const
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(LocalTransform(), scale, rot, trans, skew, perspective);

		return glm::degrees(eulerAngles(rot));
	}

	void ActorTransform::SetEulerAngles(const vec3& _new) const
	{
		vec3 scale;
		quat rot;
		vec3 trans;
		vec3 skew;
		vec4 perspective;

		decompose(LocalTransform(), scale, rot, trans, skew, perspective);

		*m_transform = translate(mat4(1.f), trans) *
			toMat4(quat(_new)) *
			glm::scale(mat4(1.f), scale);
	}

	void ActorTransform::UpdateEulerAngles(const vec3& _delta) const
	{
		*m_transform = *m_transform * toMat4(quat(_delta));
	}

	void ActorTransform::TRS(vec3 _loc, const quat& _rot, const vec3& _scale) const
	{
		SetLocation(_loc);
		SetScale(_scale);
		SetRotation(_rot);
	}

	void ActorTransform::TRS(vec3 _loc, const vec3& _euler, const vec3& _scale) const
	{
		SetLocation(_loc);
		SetScale(_scale);
		SetRotation(_euler);
	}

	void ActorTransform::LookAt(const vec3 _loc) const
	{
		SetRotation(
			quat(lookAt(Location(), _loc, Up()))
		);
	}

	vec3 ActorTransform::Forward() const
	{
		const vec3 rot = glm::radians(EulerAngles());

		return
		{
			glm::cos(rot.x) * glm::sin(rot.y),
			-glm::sin(rot.x),
			glm::cos(rot.x) * glm::cos(rot.y)
		};
	}

	vec3 ActorTransform::Right() const
	{
		const vec3 rot = glm::radians(EulerAngles());

		return
		{
			-glm::cos(rot.y),
			0,
			glm::sin(rot.y)
		};
	}

	vec3 ActorTransform::Up() const
	{
		const vec3 rot = glm::radians(EulerAngles());

		return
		{
			glm::sin(rot.x) * glm::sin(rot.y),
			glm::cos(rot.x),
			glm::sin(rot.x) * glm::cos(rot.y)
		};
	}

	ActorTransform::ActorTransform()
		: m_parent{ nullptr }, m_transform{ new mat4(1.f) }
	{
	}

	ActorTransform::~ActorTransform()
	{
		for(const auto& child : m_children)
			delete child;

		m_children.clear();

		delete m_transform;
		m_transform = nullptr;
	}

	void ActorTransform::ApplyChanges()
	{
		for (auto& [fnc, parent, child] : m_changes)
			std::invoke(fnc, parent, child);

		m_changes.clear();
	}

	void ActorTransform::AddChild(ActorTransform* _child)
	{
		if (std::ranges::find(m_children, _child) != m_children.end())
			return;

		if (_child->m_parent)
			_child->m_parent->RemoveChild(_child);

		_child->m_parent = this;
		m_children.emplace_back(_child);
	}

	void ActorTransform::RemoveChild(ActorTransform* _child)
	{
		if (std::ranges::find(m_children, _child) == m_children.end())
			return;

		if (_child->m_parent == this)
		{
			_child->m_parent = nullptr;

			const list<ActorTransform*>::iterator iter = std::ranges::find(m_children, _child);
			if (iter != m_children.end())
				m_children.erase(iter, iter);
		}
	}
}
