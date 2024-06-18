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

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>

#include <list>

#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"
#include "ActorTransform.h"

using glm::mat4;
using glm::vec3;
using glm::quat;

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
		mat4 LocalToWorld() const;
		mat4 LocalTransform() const;

		void SetParent(ActorTransform* _newParent);
		ActorTransform* Parent() const;

		list<ActorTransform*> Children() const;

		vec3 Location() const;
		vec3 LocalLocation() const;

		void SetLocation(vec3& _new) const;
		void UpdateLocation(vec3& _delta) const;

		vec3 Scale() const;
		vec3 LocalScale() const;

		void SetScale(const vec3& _new) const;
		void UpdateScale(const vec3& _delta) const;

		quat Rotation() const;
		quat LocalRotation() const;

		void SetRotation(const quat& _new) const;

		vec3 EulerAngles() const;
		vec3 LocalEulerAngles() const;

		void SetEulerAngles(const vec3& _new) const;
		void UpdateEulerAngles(const vec3& _delta) const;

		void TRS(vec3 _loc, const quat& _rot, const vec3& _scale) const;
		void TRS(vec3 _loc, const vec3& _euler, const vec3& _scale) const;

		void LookAt(vec3 _loc) const;

		vec3 Forward() const;
		vec3 Right() const;
		vec3 Up() const;

	public:
		ActorTransform& operator=(const ActorTransform&) = delete;
		ActorTransform& operator=(ActorTransform&&) = delete;

	private:
		list<tuple<HierarchyChange, ActorTransform*, ActorTransform*>> m_changes;

		list<ActorTransform*> m_children;
		ActorTransform* m_parent;

		mat4* m_transform;

	private:
		ActorTransform();
		~ActorTransform();

	private:
		void ApplyChanges();
		void AddChild(ActorTransform* _child);
		void RemoveChild(ActorTransform* _child);

	};
}