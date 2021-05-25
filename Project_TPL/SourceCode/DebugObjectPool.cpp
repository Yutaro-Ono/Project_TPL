//----------------------------------------------------------------------------------+
// @file        DebugObjectPool.h
// @brief       �f�o�b�O�I�u�W�F�N�g�v�[�� (DebugObjectBase�N���X���Ǘ�����)
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/25   �V�K�쐬
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
