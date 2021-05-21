//----------------------------------------------------------------------------------+
// @file        MeshPool.cpp
// @brief       メッシュプール (テクスチャクラスを管理する)
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/12   新規作成
//----------------------------------------------------------------------------------+
#include "MeshPool.h"
#include "MeshObj.h"
#include "MeshGpmesh.h"

MeshPool::MeshPool()
{
}

MeshPool::~MeshPool()
{
	Delete();
}

void MeshPool::UpdateObjects(float _deltaTime)
{
}

/// <summary>
/// メッシュの取得関数
/// </summary>
/// <param name="_meshPath"></param>
/// <returns> メッシュクラス </returns>
Mesh* MeshPool::GetMesh(const std::string& _meshPath)
{
	Mesh* mesh = nullptr;
	auto iter = m_keyObjPool.find(_meshPath);
	
	if (iter != m_keyObjPool.end())
	{
		return iter->second;
	}
	else
	{
		// ファイルの拡張子から、適したメッシュクラスを生成する
		// 拡張子"."が最後から何文字目にあるかを調べる → 拡張子部分を取得
		int extNum = _meshPath.find_last_of(".");
		std::string extension = _meshPath.substr(extNum, _meshPath.size() - extNum);

		// .OBJ形式
		if (extension == ".obj" || extension == ".OBJ")
		{
			mesh = new MeshObj();
			
			// メッシュのロード
			if (mesh->Load(_meshPath))
			{
				// 成功
				m_keyObjPool.emplace(_meshPath, mesh);
			}
			else
			{
				// 失敗
				delete mesh;
				mesh = nullptr;
			}
		}

		// .gpmesh形式
		if (extension == ".gpmesh")
		{
			mesh = new MeshGpmesh();

			// メッシュのロード
			if (mesh->Load(_meshPath))
			{
				// 成功
				m_keyObjPool.emplace(_meshPath, mesh);
			}
			else
			{
				// 失敗
				delete mesh;
				mesh = nullptr;
			}
		}
	}


	return mesh;
}
