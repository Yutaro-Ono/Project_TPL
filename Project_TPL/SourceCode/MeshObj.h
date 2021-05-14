//----------------------------------------------------------------------------------+
// @file        MeshObj.h
// @brief       メッシュ情報(頂点配列・テクスチャ)を保持する
//              
// @note        .obj形式ファイルの読み込みに対応
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/12   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "Mesh.h"
#include "tiny_obj_loader.h"

class MeshObj : public Mesh
{
public:

	MeshObj();
	~MeshObj();

	bool Load(const std::string& _filePath) override;


};