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

MeshComponent::MeshComponent(Actor* _owner, const std::string& _filePath)
	:Component(_owner)
	,m_mesh(nullptr)
	,m_isVisible(false)
	,m_isDrawShadow(true)
	,m_isDrawHUD(true)
	,m_hudColor(glm::vec3(1.0))
{
	Load(_filePath);
}

MeshComponent::~MeshComponent()
{
}

/// <summary>
/// メッシュのロード
/// </summary>
/// <param name="_filePath"></param>
void MeshComponent::Load(const std::string& _filePath)
{
	// メッシュプールからメッシュを取得
	m_mesh = MESH_POOL->GetMesh(_filePath);
}

/// <summary>
/// 深度マップに影を描画する
/// </summary>
/// <param name="_depthShader"> 深度マップ描画に用いるシェーダー </param>
void MeshComponent::DrawShadow(GLSLprogram* _depthShader)
{
}


/// <summary>
/// メッシュの描画処理
/// </summary>
/// <param name="_shader"> 描画に用いるシェーダークラス </param>
void MeshComponent::Draw(GLSLprogram* _shader)
{
	if (m_mesh != nullptr && !m_isVisible)
	{

	}
}
