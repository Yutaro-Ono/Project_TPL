//----------------------------------------------------------------------------------+
// @file        Component.cpp
// @brief       �R���|�[�l���g�N���X
//              
// @note        �x�[�X�N���X
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/14   �V�K�쐬
//----------------------------------------------------------------------------------+
#include "Component.h"

int Component::m_globalID = 0;

Component::Component(Actor* _owner, int _updateOrder)
	:m_owner(_owner)
	,m_updateOrder(_updateOrder)
{
	// �I�[�i�[�A�N�^�ɃR���|�[�l���g��ǉ�
	m_owner->AddComponent(this);
	// ��ID�̓o�^�ƃO���[�o��ID�̃C���N�������g
	m_ID = m_globalID;
	m_globalID++;
}

Component::~Component()
{
	m_owner->RemoveComponent(this);
}
