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
