//----------------------------------------------------------------------------------+
// @file        ActorPool.cpp
// @brief       アクタープール (Actorクラスを管理する)
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/14   新規作成
//----------------------------------------------------------------------------------+
#include "ActorPool.h"

ActorPool::ActorPool()
{
}

ActorPool::~ActorPool()
{
	Delete();
}

void ActorPool::UpdateObjects(float _deltaTime)
{
	for (auto iter : m_objPool)
	{
		iter->UpdateActor(_deltaTime);
		iter->Update(_deltaTime);
	}
}
