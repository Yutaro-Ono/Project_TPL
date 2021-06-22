#include "BarricadeActor.h"
#include "MeshComponent.h"

BarricadeActor::BarricadeActor()
	:Actor(OBJECT_TAG::STATIC_OBJECT)
{
	//MeshComponent* mc = new MeshComponent(this, "Data/Meshes/Environments/Barricade/barricade.gpmesh");

	//MeshComponent* mc = new MeshComponent(this, "Data/Meshes/Tank/tank.obj");

	MeshComponent* mc = new MeshComponent(this, "Data/Meshes/Environments/VikingRoom/viking_room.obj");
}

BarricadeActor::~BarricadeActor()
{
}

void BarricadeActor::UpdateActor(float _deltaTime)
{
}
