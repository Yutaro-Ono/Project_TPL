//----------------------------------------------------------------------------------+
// @file        DebugObjectPool.h
// @brief       デバッグオブジェクトプール (DebugObjectBaseクラスを管理する)
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/25   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "ObjectPool.h"
#include "DebugObjectBase.h"

class DebugObjectPool : public ObjectPool<class DebugObjectBase>
{

public:

	DebugObjectPool();
	~DebugObjectPool();

	void UpdateObjects(float _deltaTime) override;


private:




};