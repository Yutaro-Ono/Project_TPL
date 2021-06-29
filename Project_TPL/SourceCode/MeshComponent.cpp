//----------------------------------------------------------------------------------+
// @file        MeshComponent.cpp
// @brief       メッシュコンポーネントクラス
//              
// @note        Componentクラスを継承
//              アクターにメッシュ情報を紐付ける
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
#include "GLSLprogram.h"
#include "VertexArray.h"
#include "DrawableObjectManager.h"

MeshComponent::MeshComponent(Actor* _owner)
	:Component(_owner)
	,m_mesh(nullptr)
	,m_isVisible(false)
	,m_isDrawShadow(true)
	,m_isDrawHUD(true)
	,m_hudColor(glm::vec3(1.0))
{
	RENDERER->GetDrawableObjectManager()->AddMeshComp(this);
}

MeshComponent::MeshComponent(Actor* _owner, const std::string& _filePath)
	:Component(_owner)
	,m_mesh(nullptr)
	,m_isVisible(false)
	,m_isDrawShadow(true)
	,m_isDrawHUD(true)
	,m_hudColor(glm::vec3(1.0))
{
	RENDERER->GetDrawableObjectManager()->AddMeshComp(this);
	Load(_filePath);
}

MeshComponent::~MeshComponent()
{
	RENDERER->GetDrawableObjectManager()->DeleteMeshComp(this);
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
		// ワールド変換行列をシェーダーに送る
		_shader->SetUniform("u_worldTransform", m_owner->GetWorldTransform());


		// マテリアル情報のバインド
		SetMaterialToUniform();

		// 頂点配列オブジェクトのアクティブ化
		m_mesh->SetActiveVAO();

		// 描画
		glDrawElements(GL_TRIANGLES, m_mesh->GetVertexArray()->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

/// <summary>
/// マテリアル(テクスチャ)を指定順にバインド
/// </summary>
void MeshComponent::SetMaterialToUniform()
{
	// Albedo → Normal → Metallic → Roughness → AO → Depth
	// 指定したタイプのテクスチャがない場合は無効数値"0"がセット
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(PBR_MATERIAL::ALBEDO));

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(PBR_MATERIAL::NORMAL));

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(PBR_MATERIAL::METALLIC));

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(PBR_MATERIAL::ROUGHNESS));

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(PBR_MATERIAL::AO));

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(PBR_MATERIAL::SPECULAR));

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(PBR_MATERIAL::EMISSIVE));
}
