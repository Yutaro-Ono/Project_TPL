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
#include <GL/glew.h>
#include <typeinfo>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_types.h>
#include <SDL_mixer.h>
#include <SDL_image.h>

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
	void UpdateDeltaTime();

	// Getter
	class Renderer* GetRenderer() { return m_renderer; }
	class Debugger* GetDebugger() { return m_debugger; }
	class TexturePool* GetTexturePool() { return m_texturePool; }
	class MeshPool* GetMeshPool() { return m_meshPool; }
	class ActorPool* GetActorPool() { return m_actorPool; }
	float GetBulletTime() { return m_bulletTime; }


	// Setter
	void SetBulletTime(float _time) { m_bulletTime = _time; }
	void SetScene(class SceneBase* _scene);

private:

	GameMain();
	
	void Delete();
	void ProcessInput();

	bool m_isRunning;                      // �Q�[�����[�v�𑱂��邩

	// �����o �N���X
	class Renderer* m_renderer;            // �����_���[
	class Debugger* m_debugger;            // �f�o�b�K�\
	class SceneBase* m_scene;              // �Q�[���V�[��

	class TexturePool* m_texturePool;      // �e�N�X�`���v�[��
	class MeshPool* m_meshPool;            // ���b�V���v�[��
	class ActorPool* m_actorPool;          // �A�N�^�[�v�[��

	// �t���[���X�V�ɉe������ϐ�
	float m_deltaTime;
	Uint32 m_ticksCount;
	float m_bulletTime;
};

#define GAME_INSTANCE GameMain::GetInstance()
#define GAME_CONFIG GameSettings::GetInstance()
#define RENDERER GameMain::GetInstance().GetRenderer()
#define DEBUGGER GameMain::GetInstance().GetDebugger()
#define MESH_POOL GameMain::GetInstance().GetMeshPool()
#define TEXTURE_POOL GameMain::GetInstance().GetTexturePool()
#define ACTOR_POOL GameMain::GetInstance().GetActorPool()