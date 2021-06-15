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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
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

	void SetPosition(const glm::vec3& _pos);
	void SetScale(const glm::vec3& _scale);
	void SetEulerAngle(const glm::vec3& _angle);

	const glm::mat4& GetWorldTransform() const { return m_worldTrans; }
	const glm::vec3& GetPosition() const { return m_position; }

protected:

	ActorEnum::ACTOR_STATE m_state;

	// ���W�n
	glm::vec3 m_position;
	glm::mat4 m_worldTrans;
	// �ϊ�
	glm::vec3 m_scale;
	// ��]�s��
	glm::vec3 m_eulerAngles;
	glm::quat m_rotationX;
	glm::quat m_rotationY;
	glm::quat m_rotationZ;
	
	bool m_recomputeWorldTransform;


	std::vector<class Component*> m_components;           // �A�N�^�[���ێ�����R���|�[�l���g�N���X

	OBJECT_TAG m_tag;                                     // �A�N�^�[�^�O
	int m_ID;                                             // �A�N�^�[����ID
	static int m_globalActorNo;                           // �Q�[���S�̂ɂ�����A�N�^�[�ԍ�

	
	class ActorDebugObject* m_debugObj;                   // �A�N�^�[�p�f�o�b�O�I�u�W�F�N�g

	friend class ActorDebugObject;
};

