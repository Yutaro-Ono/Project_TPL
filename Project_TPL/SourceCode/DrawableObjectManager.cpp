//----------------------------------------------------------------------------------+
// @file        DrawableObjectManager.cpp
// @brief       描画可能オブジェクトの管理クラス
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/14   新規作成
//----------------------------------------------------------------------------------+
#include "DrawableObjectManager.h"
#include "MeshComponent.h"


DrawableObjectManager::DrawableObjectManager()
{
}

DrawableObjectManager::~DrawableObjectManager()
{
	// メッシュコンポーネント解放処理
	while (!m_meshComponents.empty())
	{
		delete m_meshComponents.back();
	}
}

/// <summary>
/// メッシュコンポーネントクラスを配列に追加する
/// </summary>
/// <param name="_meshComp"> 配列に追加するメッシュコンポーネントクラス </param>
void DrawableObjectManager::AddMeshComp(MeshComponent* _meshComp)
{
	m_meshComponents.emplace_back(_meshComp);
}

/// <summary>
/// メッシュコンポーネントクラスの削除
/// </summary>
/// <param name="_meshComp"> 削除したいメッシュコンポーネントクラス </param>
void DrawableObjectManager::DeleteMeshComp(MeshComponent* _meshComp)
{
	auto iter = std::find(m_meshComponents.begin(), m_meshComponents.end(), _meshComp);

	if (iter != m_meshComponents.end())
	{
		std::iter_swap(iter, m_meshComponents.end() - 1);
		m_meshComponents.pop_back();
	}

}

/// <summary>
/// メッシュコンポ―ネントの描画処理
/// </summary>
/// <param name="_shaderManager"> 描画に用いるシェーダークラス </param>
void DrawableObjectManager::Draw(class GLSLprogram* _shader)
{
	for (auto mc : m_meshComponents)
	{
		mc->Draw(_shader);
	}
}
