//  ***************************************************************
//  ActorComponent - Creation date: 27/03/2025 02:01:33 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"

#include "ActorComponent.h"
#include "IRenderFeature.h"

#include <list>
#include <string>

using std::list;
using std::string;

namespace Catalyst
{
	class Mesh;
	class Material;

	class DLL StaticMeshComponent : public ActorComponent, public IRenderFeature
	{
	public:
		list<Mesh*> submeshes;
		list<Material*> materials;

	public:
		StaticMeshComponent();
		~StaticMeshComponent() override;

	public:
		void LoadMesh(const string& _mesh);
		void SetMaterial(int _slot, Material* _material);

	protected:
		virtual void Render() override;

	private:
		Material* GetMaterialFor(Mesh* mesh);

	};
}