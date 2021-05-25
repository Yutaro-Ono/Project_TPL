//----------------------------------------------------------------------------------+
// @file        DebugObjectPool.h
// @brief       �f�o�b�O�I�u�W�F�N�g�v�[�� (DebugObjectBase�N���X���Ǘ�����)
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/25   �V�K�쐬
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