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
	sphere->SetPosition(glm::vec3(10.0f, 5.0f, 10.0f));
	sphere->SetScale(glm::vec3(1.4f));
	TestSphere* sphere1 = new TestSphere();
	sphere1->SetPosition(glm::vec3(-10.0f, 0.0f, 7.0f));
	sphere1->SetScale(glm::vec3(0.5f));
	TestSphere* sphere2 = new TestSphere();
	sphere2->SetPosition(glm::vec3(4.0f, -5.0f, 1.0f));
	sphere2->SetScale(glm::vec3(0.3f));

	BarricadeActor* barricade1 = new BarricadeActor();
	barricade1->SetScale(glm::vec3(0.1f));
}

SceneBase* TestScene::Update(float _deltaTime)
{
    return this;
}
