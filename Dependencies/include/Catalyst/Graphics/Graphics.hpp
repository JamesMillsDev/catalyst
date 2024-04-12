#pragma once

#include <Catalyst/Catalyst.hpp>
#include <Catalyst/Engine/IModule.hpp>

#include <list>
#include <vector>

#include <glm/vec3.hpp>

using std::list;
using std::pair;
using std::vector;

using glm::vec3;

namespace Catalyst
{
	class Graphics final : public IModule
	{
		friend class Camera;
		friend class LightComponent;
		friend class Material;

		typedef void(Graphics::* CameraListChange)(class Camera*);
		typedef void(Graphics::* LightListChange)(class LightComponent*);

	public:
		DLL Graphics();

		DLL class Camera* MainCamera() const;
		DLL void SetMainCamera(class Camera* _cam);

		DLL list<class LightComponent*> Lights();
		DLL LightComponent* GetLight(size_t _index);

	protected:
		void Tick(Application* _app) override;

	private:
		class Camera* m_main;
		list<class Camera*> m_cameras;
		list<class LightComponent*> m_lightComponents;

		list<pair<CameraListChange, class Camera*>> m_cameraListChanges;
		list<pair<LightListChange, class LightComponent*>> m_lightListChanges;

	private:
		DLL void Add(class Camera* _camera);
		DLL void Remove(class Camera* _camera);
		DLL void Add(class LightComponent* _light);
		DLL void Remove(class LightComponent* _light);

		DLL void AddCamera(class Camera* _camera);
		DLL void RemoveCamera(class Camera* _camera);
		DLL void AddLight(class LightComponent* _light);
		DLL void RemoveLight(class LightComponent* _light);

		DLL int LightCount(bool _includeDirectional = false) const;

	};
}
