#include "BarricadeActor.h"
#include "MeshComponent.h"

BarricadeActor::BarricadeActor()
	:Actor(OBJECT_TAG::STATIC_OBJECT)
{
	MeshComponent* mc = new MeshComponent(this, "Data/Meshes/Barricade/barricade.gpmesh");
}

BarricadeActor::~BarricadeActor()
{
}

void BarricadeActor::UpdateActor(float _deltaTime)
{
}
