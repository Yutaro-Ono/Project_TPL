//----------------------------------------------------------------------------------+
// @file        Component.h
// @brief       �R���|�[�l���g�N���X
//              
// @note        �x�[�X�N���X
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/14   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "Actor.h"

class Component
{

public:

	Component(class Actor* _owner, int _updateOrder = 100);
	virtual ~Component();

	virtual void Update(float _deltaTime) {}
	virtual void ProcessInput(float _deltaTime) {}
	virtual void OnUpdateTransform() {}

	int GetUpdateOrder() { return m_updateOrder; }

protected:


	class Actor* m_owner;

	int m_updateOrder;

	int m_ID;
	static int m_globalID;

};