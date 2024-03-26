#pragma once

#include <Catalyst/Catalyst.hpp>
#include <Catalyst/Engine/IModule.hpp>

#include <list>

using std::list;
using std::pair;

namespace Catalyst
{
	class Graphics final : public IModule
	{
		friend class Camera;

		typedef void(Graphics::* CameraListChange)(Camera*);

	public:
		DLL Graphics();

		DLL Camera* MainCamera() const;
		DLL void SetMainCamera(Camera* _cam);

	protected:
		void Tick(BaseApplication* _app) override;

	private:
		Camera* m_main;
		list<Camera*> m_cameras;

		list<pair<CameraListChange, Camera*>> m_cameraListChanges;

	private:
		DLL void Add(Camera* _camera);
		DLL void Remove(Camera* _camera);

		DLL void AddCamera(Camera* _camera);
		DLL void RemoveCamera(Camera* _camera);

	};
}
