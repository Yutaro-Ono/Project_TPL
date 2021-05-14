//----------------------------------------------------------------------------------+
// @file        MeshComponent.cpp
// @brief       ���b�V���R���|�[�l���g�N���X
//              
// @note        Component�N���X���p��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/14   �V�K�쐬
//----------------------------------------------------------------------------------+
#include "MeshComponent.h"
#include "Mesh.h"
#include "GameMain.h"
#include "Renderer.h"
#include "MeshPool.h"

MeshComponent::MeshComponent(Actor* _owner)
	:Component(_owner)
	,m_mesh(nullptr)
	,m_isVisible(false)
	,m_isDrawShadow(true)
	,m_isDrawHUD(true)
	,m_hudColor(glm::vec3(1.0))
{
	
}

MeshComponent::~MeshComponent()
{
}

void MeshComponent::DrawShadow(GLSLprogram* _shader)
{
}

void MeshComponent::Draw(GLSLprogram* _shader)
{
	if (m_mesh != nullptr && !m_isVisible)
	{

	}
}
