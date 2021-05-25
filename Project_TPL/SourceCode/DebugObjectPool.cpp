//----------------------------------------------------------------------------------+
// @file        DebugObjectPool.h
// @brief       デバッグオブジェクトプール (DebugObjectBaseクラスを管理する)
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/25   新規作成
//----------------------------------------------------------------------------------+
#include "DebugObjectPool.h"

DebugObjectPool::DebugObjectPool()
{
}

DebugObjectPool::~DebugObjectPool()
{
}

void DebugObjectPool::UpdateObjects(float _deltaTime)
{
	for (auto iter : m_objPool)
	{
		iter->Update(_deltaTime);
	}
}
