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
