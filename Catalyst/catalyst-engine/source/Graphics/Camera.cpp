#include <Catalyst/Graphics/Camera.hpp>

#include <Catalyst/Gameplay/Actors/Transform.hpp>

#include <glfw/glfw3.h>
#include <glm/ext.hpp>

namespace Catalyst
{
	Camera::Camera(const float _fov, const float _near, const float _far)
		: m_transform{ std::make_shared<Transform>(Transform({ -10.f, 2.f, 0.f }, vec3(1), vec3(0))) },
		m_viewMat{ mat4(1) }, m_projMat{ mat4(1.f) }, m_fov{ _fov }, m_near{ _near }, m_far{ _far }
	{

	}

	Camera::~Camera()
	{
		m_transform.reset();
		/*m_frustrum.reset();*/
	}

	/*bool Camera::Visible(Volume* _volume, shared_ptr<class Transform> _transform)
	{
		return _volume->Visible(m_frustrum, _transform);
	}*/

	mat4 Camera::ProjectTransform(const mat4& _mat)
	{
		return ProjectionView() * _mat;
	}

	mat4 Camera::ProjectionView()
	{
		m_viewMat = glm::lookAt(Location(), Location() + Forward(), Up());
		m_projMat = glm::perspective(Fov(), Aspect(), m_near, m_far);

		return m_projMat * m_viewMat;
	}

	float Camera::Fov() const
	{
		return m_fov * (glm::pi<float>() / 180.f);
	}

	float Camera::Near() const
	{
		return m_near;
	}

	float Camera::Far() const
	{
		return m_far;
	}

	float Camera::Aspect() const
	{
		int w = 0;
		int h = 0;

		const auto context = glfwGetCurrentContext();
		glfwGetWindowSize(context, &w, &h);

		const float width = static_cast<float>(w);
		const float height = static_cast<float>(h);

		return width / height;
	}

	vec3 Camera::Forward() const
	{
		return m_transform->Forward();
	}

	vec3 Camera::Right() const
	{
		return m_transform->Right();
	}

	vec3 Camera::Up() const
	{
		return m_transform->Up();
	}

	vec3 Camera::Location() const
	{
		return m_transform->Location();
	}
}