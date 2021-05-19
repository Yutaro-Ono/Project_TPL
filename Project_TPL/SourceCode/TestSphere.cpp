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
{
	MeshComponent* mc = new MeshComponent(this, "Data/Meshes/Sphere/Sphere.obj");
}

TestSphere::~TestSphere()
{
}

void TestSphere::UpdateActor(float _deltaTime)
{
}
