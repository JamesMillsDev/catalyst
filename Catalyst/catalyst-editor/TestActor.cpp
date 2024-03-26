#include "TestActor.h"

#include "Catalyst/Graphics/Components/StaticMeshComponent.hpp"

TestActor::TestActor()
{
	CreateComponent<Catalyst::StaticMeshComponent>();
}
