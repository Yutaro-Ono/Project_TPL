//----------------------------------------------------------------------------------+
// @file        CameraActor.h
// @brief       �J�����A�N�^�[�N���X
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 6/04   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "Actor.h"



class CameraActor : public Actor
{

public:


	CameraActor();
	~CameraActor();

	void UpdateActor(float _deltaTime) override;

	void SetTargetActor(class Actor* _target);
	void SetCameraLength(const glm::vec3& _length) { m_length = _length; }

private:

	void CreateViewMatrix();


	glm::mat4 m_viewMat;

	class Actor* m_targetActor;
	glm::vec3 m_targetPos;
	bool m_targetExists;

	glm::vec3 m_length;
	float m_rotateAngle;
	float m_upAngle;


};