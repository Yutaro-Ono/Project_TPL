//----------------------------------------------------------------------------------+
// @file        GameMain.h
// @brief       �Q�[�����[�v�Ɋւ��C���X�^���X�𐶐��E�ێ�
//              �X�V�������s��
// @note        �V���O���g�������Ainclude����΂ǂ�����ł��A�N�Z�X�\
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/21   �V�K�쐬
//
//----------------------------------------------------------------------------------+
#pragma once
// �C���N���[�h�t�@�C��
// ��gl3w��glfw�̏��ԂɋL�q
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

class GameMain
{

public:

	// �V���O���g���p�^�[��
	// GameMain�N���X�̃��[�J���ÓI�I�u�W�F�N�g��Ԃ�
	static GameMain& GetInstance()
	{
		static GameMain game;
		return game;
	}

	~GameMain();

	bool Initialize();
	bool RunLoop();


private:

	GameMain();
	
	void Delete();
	void ProcessInput();


	// �����o �C���X�^���X
	class Renderer* m_renderer;     // �����_���[
	class Debugger* m_debugger;     // �f�o�b�K�\
	class SceneBase* m_scene;       // �Q�[���V�[��

};

#define GAME_INSTANCE GameMain::GetInstance()