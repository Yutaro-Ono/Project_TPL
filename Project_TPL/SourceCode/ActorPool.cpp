//----------------------------------------------------------------------------------+
// @file        ActorPool.cpp
// @brief       �A�N�^�[�v�[�� (Actor�N���X���Ǘ�����)
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/14   �V�K�쐬
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
