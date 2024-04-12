#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <Catalyst/Catalyst.hpp>

#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <list>
#include <tuple>

using glm::quat;
using glm::vec3;
using glm::mat4;

using std::list;
using std::tuple;

namespace Catalyst
{
	class Transform final
	{
		friend class ActorManager;

		typedef void(Transform::* ActorTransformListChange)(Transform*);

	public:
		DLL Transform();
		DLL Transform(const mat4& _transform);
		DLL Transform(vec3 _position, vec3 _size, vec3 _rotation);

		DLL Transform(const Transform& _other) = delete;
		DLL Transform(Transform&& _other) noexcept;

		DLL ~Transform();

	public:
		DLL mat4 Global();
		DLL mat4 Local();

		DLL void SetParent(const Transform* _parent);
		DLL Transform* Parent() const;
		DLL list<Transform*> Children();

		DLL vec3 Location();
		DLL vec3 LocalLocation();

		DLL void SetLocation(const vec3& _newLocation);
		DLL void UpdateLocation(const vec3& _amount);

		DLL vec3 Scale();
		DLL vec3 LocalScale();

		DLL void SetScale(const vec3& _newScale);
		DLL void UpdateScale(const vec3& _amount);

		DLL vec3 Rotation();
		DLL vec3 LocalRotation();

		DLL void SetRotation(const vec3& _newRot);
		DLL void SetRotation(const quat& _newRot);
		DLL void UpdateRotation(const vec3& _amount);

		DLL void TRS(const vec3& _loc, const vec3& _angle, const vec3& _scale);

		DLL vec3 Forward();
		DLL vec3 Right();
		DLL vec3 Up();

	private:
		list<tuple<ActorTransformListChange, Transform*, Transform*>> m_listChanges;

		list<Transform*> m_children;
		Transform* m_parent;

		mat4* m_transform;

	private:
		DLL void Tick(float _deltaTime);

		DLL void AddChild(Transform* _child);
		DLL void RemoveChild(Transform* _child);
	};
}