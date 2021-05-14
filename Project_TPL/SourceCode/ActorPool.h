//----------------------------------------------------------------------------------+
// @file        ActorPool.h
// @brief       �A�N�^�[�v�[�� (Actor�N���X���Ǘ�����)
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/14   �V�K�쐬
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