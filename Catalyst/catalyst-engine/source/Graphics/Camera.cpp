#include <Catalyst/Graphics/Camera.hpp>

#include <glfw/glfw3.h>
#include <glm/ext.hpp>

namespace Catalyst
{
	Camera::Camera(const float _fov, const float _near, const float _far)
		: m_fov{ _fov }, m_near{ _near }, m_far{ _far }, m_theta{ 0 }, m_phi{ 0 }
	{
		m_position = { -10.f, 2.f, 0.f };

		const float thetaR = glm::radians(m_theta);
		const float phiR = glm::radians(m_phi);
		const vec3 forward(glm::cos(phiR) * glm::cos(thetaR), glm::sin(phiR), glm::cos(phiR) * glm::sin(thetaR));

		m_viewMat = glm::lookAt(m_position, m_position + forward, { 0, 1, 0 });
		m_projMat = glm::perspective(Fov(), Aspect(), m_near, m_far);
	}

	Camera::~Camera()
	{
		/*m_transform.reset();
		m_frustrum.reset();*/
	}

	/*bool Camera::Visible(Volume* _volume, shared_ptr<class Transform> _transform)
	{
		return _volume->Visible(m_frustrum, _transform);
	}*/

	mat4 Camera::ProjectTransform(const mat4& _mat) const
	{
		return m_projMat * m_viewMat * _mat;
	}

	mat4 Camera::ProjectionView() const
	{
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

		auto context = glfwGetCurrentContext();
		glfwGetWindowSize(context, &w, &h);

		const float width = static_cast<float>(w);
		const float height = static_cast<float>(h);

		return width / height;
	}

	/*vec3 Camera::Forward() const
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
	}*/

	void Camera::Tick()
	{
		const float thetaR = glm::radians(m_theta);
		const float phiR = glm::radians(m_phi);
		const vec3 forward(glm::cos(phiR) * glm::cos(thetaR), glm::sin(phiR), glm::cos(phiR) * glm::sin(thetaR));

		m_viewMat = glm::lookAt(m_position, m_position + forward, { 0, 1, 0 });
		m_projMat = glm::perspective(Fov(), Aspect(), m_near, m_far);

		/*Frustrum::Update(this, m_frustrum);*/
	}
}