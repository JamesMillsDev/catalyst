#pragma once

#include <Catalyst/Catalyst.hpp>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

using glm::mat4;
using glm::vec3;

namespace Catalyst
{
	class Camera
	{
	public:
		DLL Camera(float _fov, float _near, float _far);
		DLL virtual ~Camera();

	public:
		DLL mat4 ProjectTransform(const mat4& _mat) const;
		DLL mat4 ProjectionView() const;

		DLL float Fov() const;
		DLL float Near() const;
		DLL float Far() const;
		DLL float Aspect() const;

		/*DLL vec3 Forward() const;
		DLL vec3 Right() const;
		DLL vec3 Up() const;
		DLL vec3 Location() const;*/

	protected:
		mat4 m_viewMat;
		mat4 m_projMat;

		float m_fov;
		float m_near;
		float m_far;

		float m_theta;
		float m_phi;

		vec3 m_position;

	protected:
		DLL virtual void Tick();

	};
}
