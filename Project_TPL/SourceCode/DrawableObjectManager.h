//----------------------------------------------------------------------------------+
// @file        DrawableObjectManager.h
// @brief       描画可能オブジェクトの管理クラス
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/14   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include <vector>
#include "ShaderManager.h"

class DrawableObjectManager
{

public:

	DrawableObjectManager();
	~DrawableObjectManager();



	void AddMeshComp(class MeshComponent* _meshComp);
	void DeleteMeshComp(class MeshComponent* _meshComp);

	void Draw(class GLSLprogram* _shader);





private:

	std::vector<class MeshComponent*> m_meshComponents;

};