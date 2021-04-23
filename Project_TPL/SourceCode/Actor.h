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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

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

	Actor();
	virtual ~Actor();

	void Update(float _deltaTime);
	void UpdateComponents(float _deltaTime);
	virtual void UpdateActor(float _deltaTime);

	void CalcWorldTransform();

private:

	ActorEnum::ACTOR_STATE m_state;

	// ���W�n
	glm::vec3 m_position;
	glm::mat4 m_worldTrans;
	// �ϊ�
	glm::vec3 m_scale;
	glm::quat m_quaternion;
	

};