//----------------------------------------------------------------------------------+
// @file        TestSphere.cpp
// @brief       テスト球体クラス
// @note        アクター
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/19   新規作成
//----------------------------------------------------------------------------------+
#include "TestSphere.h"
#include "MeshComponent.h"


TestSphere::TestSphere()
	:m_debugObj(nullptr)
{
	MeshComponent* mc = new MeshComponent(this, "Data/Meshes/Sphere/Sphere.obj");
	//MeshComponent* mc = new MeshComponent(this, "Data/Meshes/Barricade/barricade.gpmesh");
	//m_worldTrans = glm::rotate(m_worldTrans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

}

TestSphere::~TestSphere()
{
}

void TestSphere::UpdateActor(float _deltaTime)
{
	


}
