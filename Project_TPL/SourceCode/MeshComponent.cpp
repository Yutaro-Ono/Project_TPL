//----------------------------------------------------------------------------------+
// @file        MeshComponent.cpp
// @brief       メッシュコンポーネントクラス
//              
// @note        Componentクラスを継承
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/14   新規作成
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
