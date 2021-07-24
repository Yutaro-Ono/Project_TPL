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
	void SetCameraLength(const Vector3& _length) { m_length = _length; }

private:

	void CreateViewMatrix();


	Matrix4 m_viewMat;

	class Actor* m_targetActor;
	Vector3 m_targetPos;
	bool m_targetExists;

	Vector3 m_length;
	float m_rotateAngle;
	float m_upAngle;


};