//----------------------------------------------------------------------------------+
// @file        ActorPool.h
// @brief       アクタープール (Actorクラスを管理する)
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/14   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "ObjectPool.h"
#include "Actor.h"

class ActorPool : public ObjectPool<Actor>
{

public:

	ActorPool();
	~ActorPool();

	void UpdateObjects(float _deltaTime) override;

private:



};