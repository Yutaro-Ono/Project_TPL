//----------------------------------------------------------------------------------+
// @file        TestSphere.cpp
// @brief       �e�X�g���̃N���X
// @note        �A�N�^�[
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/19   �V�K�쐬
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
