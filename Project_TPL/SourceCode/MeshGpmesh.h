//----------------------------------------------------------------------------------+
// @file        MeshGpmesh.h
// @brief       メッシュ情報(頂点配列・テクスチャ)を保持する
//              
// @note        .gpmesh形式ファイルの読み込みに対応
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/21   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "Mesh.h"
#include <document.h>


class MeshGpmesh : public Mesh
{

public:

	MeshGpmesh();
	~MeshGpmesh();

	bool Load(const std::string& _filePath) override;


private:



	float m_specular;
	std::string m_shaderName;

};