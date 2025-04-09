#include "cagameplaypch.h"
#include "ActorTransform.h"

namespace Catalyst
{
	Matrix4 ActorTransform::LocalToWorld() const
	{
		return m_parent ? *m_transform * m_parent->LocalTransform() : LocalTransform();
	}

	Matrix4 ActorTransform::LocalTransform() const
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

	Vector3 ActorTransform::Location() const
	{
		return LocalToWorld().Translation();
	}

	Vector3 ActorTransform::LocalLocation() const
	{
		return LocalTransform().Translation();
	}

	void ActorTransform::SetLocation(const Vector3& _new) const
	{
		m_transform->SetTranslation(_new);
	}

	void ActorTransform::UpdateLocation(const Vector3& _delta) const
	{
		m_transform->Translate(_delta);
	}

	Vector3 ActorTransform::Scale() const
	{
		return LocalToWorld().Scale();
	}

	Vector3 ActorTransform::LocalScale() const
	{
		return LocalTransform().Translation();
	}

	void ActorTransform::SetScale(const Vector3& _new) const
	{
		m_transform->SetScale(_new);
	}

	void ActorTransform::UpdateScale(const Vector3& _delta) const
	{
		m_transform->ScaleBy(_delta);
	}

	quat ActorTransform::Rotation() const
	{
		return LocalToWorld().Rotation();
	}

	quat ActorTransform::LocalRotation() const
	{
		return LocalTransform().Rotation();
	}

	void ActorTransform::SetRotation(const quat& _new) const
	{
		m_transform->SetRotation(_new);
	}

	Vector3 ActorTransform::EulerAngles() const
	{
		return LocalToWorld().Euler();
	}

	Vector3 ActorTransform::LocalEulerAngles() const
	{
		return LocalTransform().Translation();
	}

	void ActorTransform::SetEulerAngles(const Vector3& _new) const
	{
		m_transform->SetEuler(_new);
	}

	void ActorTransform::UpdateEulerAngles(const Vector3& _delta) const
	{
		m_transform->Rotate(_delta);
	}

	void ActorTransform::TRS(const Vector3& _loc, const quat& _rot, const Vector3& _scale) const
	{
		m_transform->TRS(_loc, _rot, _scale);
	}

	void ActorTransform::TRS(const Vector3& _loc, const Vector3& _euler, const Vector3& _scale) const
	{
		m_transform->TRS(_loc, _euler, _scale);
	}

	void ActorTransform::LookAt(const Vector3& _loc) const
	{
		m_transform->LookAt(_loc);
	}

	Vector3 ActorTransform::Forward() const
	{
		return LocalToWorld().Forward();
	}

	Vector3 ActorTransform::Right() const
	{
		return LocalToWorld().Right();
	}

	Vector3 ActorTransform::Up() const
	{
		return LocalToWorld().Up();
	}

	ActorTransform::ActorTransform()
		: m_parent{ nullptr }, m_transform{ new Matrix4(1.f) }
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
