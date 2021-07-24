#include "TestScene.h"
#include "TestSphere.h"
#include "BarricadeActor.h"
#include "GameMain.h"
#include "Renderer.h"
#include "DirectionalLight.h"

TestScene::TestScene()
{
}

TestScene::~TestScene()
{
}

void TestScene::Initialize()
{
	

	TestSphere* sphere = new TestSphere();
	sphere->SetPosition(Vector3(10.0f, 5.0f, 10.0f));
	sphere->SetScale(Vector3(1.4f, 1.4f, 1.4f));
	TestSphere* sphere1 = new TestSphere();
	sphere1->SetPosition(Vector3(-10.0f, 0.0f, 7.0f));
	sphere1->SetScale(Vector3(0.5f, 0.5f, 0.5f));
	TestSphere* sphere2 = new TestSphere();
	sphere2->SetPosition(Vector3(4.0f, -5.0f, 1.0f));
	sphere2->SetScale(Vector3(0.3f, 0.3f, 0.3f));

	BarricadeActor* barricade1 = new BarricadeActor();
	barricade1->SetScale(Vector3(0.1f, 0.1f, 0.1f));
}

SceneBase* TestScene::Update(float _deltaTime)
{
    return this;
}
