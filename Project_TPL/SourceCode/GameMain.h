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


	class Renderer* GetRenderer() { return m_renderer; }
	class TexturePool* GetTexturePool() { return m_texturePool; }

private:

	GameMain();
	
	void Delete();
	void ProcessInput();


	// �����o �N���X
	class GameSettings* m_settings;        // �Q�[���ݒ�
	class Renderer* m_renderer;            // �����_���[
	class Debugger* m_debugger;            // �f�o�b�K�\
	class SceneBase* m_scene;              // �Q�[���V�[��

	class TexturePool* m_texturePool;      // �e�N�X�`���v�[��
	class MeshPool* m_meshPool;            // ���b�V���v�[��

};

#define GAME_INSTANCE GameMain::GetInstance()
#define RENDERER GameMain::GetRenderer()