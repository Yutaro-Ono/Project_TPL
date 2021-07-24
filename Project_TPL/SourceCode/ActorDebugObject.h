//----------------------------------------------------------------------------------+
// @file        ActorDebugObject.h
// @brief       �A�N�^�[�p�f�o�b�O�I�u�W�F�N�g�N���X
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/25   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
// �C���N���[�h�t�@�C��
// ��gl3w��glfw�̏��ԂɋL�q
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "DebugObjectBase.h"

class ActorDebugObject : public DebugObjectBase
{

public:


	ActorDebugObject(class Actor* _owner);
	~ActorDebugObject();

	void Update(float _deltaTime) override;
	void Render() override;

private:


	class Actor* m_owner;

	std::string m_actorLabel;      // imGui��Ŏ��ʂ��邽�߂ɗp����A�N�^�[���x��
};