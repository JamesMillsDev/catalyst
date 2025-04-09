#include "cagraphicspch.h"
#include "Components/CameraComponent.h"

#include <GLFW/glfw3.h>

#include "Actor.h"
#include "ActorTransform.h"
#include "Application.h"
#include "GraphicsModule.h"

namespace Catalyst
{
    CameraComponent::CameraComponent()
	    : m_viewMat{ mat4(1) }, m_projMat{ mat4(1.f) }, m_fovAngle{ 0 }, m_near{ 0 }, m_far{ 0 }
    {
        if (GraphicsModule* graphics = Application::GetModule<GraphicsModule>())
            graphics->Register(this);
    }

    CameraComponent::~CameraComponent() = default;

    Matrix4 CameraComponent::ProjectTransform(const Matrix4& _mat)
    {
        return ProjectionView() * _mat;
    }

    Matrix4 CameraComponent::ProjectionView()
    {
        if (const ActorTransform* transform = Owner()->Transform())
        {
            m_viewMat = transform->LocalTransform().Inverse();
            m_projMat = Matrix4::Perspective(Fov(), Aspect(), m_near, m_far);
        }

        return m_projMat * m_viewMat;
    }

    void CameraComponent::SetFovAngle(const float _fovAngle)
    {
        m_fovAngle = _fovAngle;
    }

    float CameraComponent::GetFovAngle() const
    {
        return m_fovAngle;
    }

    void CameraComponent::SetNearPlane(const float _near)
    {
        m_near = _near;
    }

    float CameraComponent::GetNearPlane() const
    {
        return m_near;
    }

    void CameraComponent::SetFarPlane(const float _far)
    {
        m_far = _far;
    }

    float CameraComponent::GetFarPlane() const
    {
        return m_far;
    }

    float CameraComponent::Fov() const
    {
        return m_fovAngle * (glm::pi<float>() / 180.f);
    }

    float CameraComponent::Aspect() const
    {
        int w = 0;
        int h = 0;

        const auto context = glfwGetCurrentContext();
        glfwGetWindowSize(context, &w, &h);

        const float width = static_cast<float>(w);
        const float height = static_cast<float>(h);

        return width / height;
    }
}
