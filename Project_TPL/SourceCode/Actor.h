//----------------------------------------------------------------------------------+
// @file        Actor.h
// @brief       �A�N�^�[�N���X (���W�������A�`�悳���I�u�W�F�N�g�S�ʂ̊��N���X)
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/23   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include <vector>
#include "Math.h"
#include "Tag.h"
#include "Component.h"


namespace ActorEnum
{
	enum ACTOR_STATE
	{
		ACTIVE,
		PAUSED,
		DEAD,
	};
}

class Actor
{

public:

	Actor(OBJECT_TAG _tag);
	virtual ~Actor();

	void Update(float _deltaTime);
	void UpdateComponents(float _deltaTime);
	virtual void UpdateActor(float _deltaTime);

	void ComputeWorldTransform();

	// �R���|�[�l���g�̒ǉ��E�폜����
	void AddComponent(class Component* _comp);
	void RemoveComponent(class Component* _comp);

	void SetPosition(const Vector3& _pos);
	void SetScale(const Vector3& _scale);
	void RotateToNewForward(const Vector3& _forward);
	void SetRotation(const Quaternion& _rot);

	const Vector3& GetPosition() const { return m_position; }
	const Matrix4& GetWorldTransform() const { return m_worldTrans; }

protected:

	ActorEnum::ACTOR_STATE m_state;

	// ���W�n
	Vector3 m_position;
	Matrix4 m_worldTrans;
	// �ϊ�
	Vector3 m_scale;
	// ��]�s��
	Quaternion m_rotation;
	
	bool m_recomputeWorldTransform;


	std::vector<class Component*> m_components;           // �A�N�^�[���ێ�����R���|�[�l���g�N���X

	OBJECT_TAG m_tag;                                     // �A�N�^�[�^�O
	int m_ID;                                             // �A�N�^�[����ID
	static int m_globalActorNo;                           // �Q�[���S�̂ɂ�����A�N�^�[�ԍ�

	
	class ActorDebugObject* m_debugObj;                   // �A�N�^�[�p�f�o�b�O�I�u�W�F�N�g

	friend class ActorDebugObject;
};

