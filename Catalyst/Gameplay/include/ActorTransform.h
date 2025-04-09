//  ***************************************************************
//  ActorTransform - Creation date: 6/12/2024 11:14:26 AM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"

#include "Mathematics/Vector3.h"
#include "Mathematics/Matrix4.h"

#include <list>

using std::list;
using std::tuple;

namespace Catalyst
{
	class DLL ActorTransform
	{
		friend class Actor;

#ifdef IS_EDITOR
		friend class EditorApplication;
#endif

		typedef void(ActorTransform::* HierarchyChange)(ActorTransform*);

	public:
		ActorTransform(const ActorTransform&) = delete;
		ActorTransform(ActorTransform&&) = delete;

	public:
		Matrix4 LocalToWorld() const;
		Matrix4 LocalTransform() const;

		void SetParent(ActorTransform* _newParent);
		ActorTransform* Parent() const;

		list<ActorTransform*> Children() const;

		Vector3 Location() const;
		Vector3 LocalLocation() const;

		void SetLocation(const Vector3& _new) const;
		void UpdateLocation(const Vector3& _delta) const;

		Vector3 Scale() const;
		Vector3 LocalScale() const;

		void SetScale(const Vector3& _new) const;
		void UpdateScale(const Vector3& _delta) const;

		quat Rotation() const;
		quat LocalRotation() const;

		void SetRotation(const quat& _new) const;

		Vector3 EulerAngles() const;
		Vector3 LocalEulerAngles() const;

		void SetEulerAngles(const Vector3& _new) const;
		void UpdateEulerAngles(const Vector3& _delta) const;

		void TRS(const Vector3& _loc, const quat& _rot, const Vector3& _scale) const;
		void TRS(const Vector3& _loc, const Vector3& _euler, const Vector3& _scale) const;

		void LookAt(const Vector3& _loc) const;

		Vector3 Forward() const;
		Vector3 Right() const;
		Vector3 Up() const;

	public:
		ActorTransform& operator=(const ActorTransform&) = delete;
		ActorTransform& operator=(ActorTransform&&) = delete;

	private:
		list<tuple<HierarchyChange, ActorTransform*, ActorTransform*>> m_changes;

		list<ActorTransform*> m_children;
		ActorTransform* m_parent;

		Matrix4* m_transform;

	private:
		ActorTransform();
		~ActorTransform();

	private:
		void ApplyChanges();
		void AddChild(ActorTransform* _child);
		void RemoveChild(ActorTransform* _child);

	};
}