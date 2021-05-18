//----------------------------------------------------------------------------------+
// @file        MeshPool.h
// @brief       メッシュプール (テクスチャクラスを管理する)
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/12   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "ObjectPool.h"
#include "Mesh.h"
#include <iostream>

class MeshPool : public ObjectPool<Mesh>
{
public:

	MeshPool();
	~MeshPool();

	void UpdateObjects(float _deltaTime) override;

	class Mesh* GetMesh(const std::string& _meshPath);


};