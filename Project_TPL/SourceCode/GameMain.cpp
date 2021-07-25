//----------------------------------------------------------------------------------+
// @file        GameMain.h
// @brief       �Q�[�����[�v�Ɋւ��C���X�^���X�𐶐��E�ێ�
//              �X�V�������s��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//----------------------------------------------------------------------------------+
#include "GameMain.h"
#include "GameSettings.h"
#include "Renderer.h"
#include "Debugger.h"
#include "SceneBase.h"
#include "TexturePool.h"
#include "MeshPool.h"
#include "ActorPool.h"
#include "InputKeyBoard.h"
#include <iostream>


/// <summary>
/// �R���X�g���N�^
/// </summary>
GameMain::GameMain()
	:m_isRunning(true)
	,m_texturePool(nullptr)
	,m_meshPool(nullptr)
	,m_actorPool(nullptr)
	,m_renderer(nullptr)
	,m_debugger(nullptr)
	,m_scene(nullptr)
	,m_deltaTime(0.0f)
	,m_ticksCount(0)
	,m_bulletTime(0.0f)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
GameMain::~GameMain()
{
	Delete();
}

/// <summary>
/// ���C�u�����������E�e��C���X�^���X�̐���
/// </summary>
/// <returns> ���������s����false��Ԃ� </returns>
bool GameMain::Initialize()
{
	// �Q�[���ݒ�N���X�̓ǂݍ��ݏ���
	if (!GAME_CONFIG.Load("Project_TPL.ini"))
	{
		std::cout << "Error::GameSettings::Load" << std::endl;
		return false;
	}

	// SDL������
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf("SDL���������s: %s\n", SDL_GetError());
		return false;
	}
	// �T�E���h�̏�����
	if (!Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG))
	{
		return false;
	}
	printf("SDLMixer����������\n");
	// SDLMixer API�������@44100:�����̎��g�� 2:�X�e���I 4096:�����o�b�t�@�T�C�Y
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		Mix_CloseAudio();
		Mix_Quit();
		return false;
	}
	int decordNum = Mix_GetNumMusicDecoders();
	for (int i = 0; i < decordNum; ++i)
	{
		printf("MusicDecorder %d : %s\n", i, Mix_GetMusicDecoder(i));
	}
	int chunkNum = Mix_GetNumChunkDecoders();
	for (int i = 0; i < chunkNum; ++i)
	{
		printf("SoundDecorder %d : %s\n", i, Mix_GetChunkDecoder(i));
	}
	printf("SDLMixerAPI����������\n");



	// �e��I�u�W�F�N�g�v�[���̐���
	m_texturePool = new TexturePool();
	m_meshPool = new MeshPool();
	m_actorPool = new ActorPool();

	// �����_���[�N���X�̍쐬
	m_renderer = new Renderer();
	if (!m_renderer->Initialize(GAME_CONFIG.m_displayWidth, GAME_CONFIG.GetInstance().m_displayHeight, GAME_CONFIG.m_displayFullScreen))
	{
		std::cout << "Error::Renderer::Initialize" << std::endl;
		return false;
	}

#ifdef _DEBUG

	// �f�o�b�K�\�N���X�̍쐬
	m_debugger = new Debugger();
	if (!m_debugger->Initialize())
	{
		std::cout << "Error::Debugger::Initialize" << std::endl;
		return false;
	}
	// �����_���[�֘A�f�o�b�O�I�u�W�F�N�g�̐���
	m_renderer->CreateRendererDebugObject();

#endif

	// �����_���[�N���X�̃��[�h���� ���f�o�b�K�[�𐶐�������
	if (!m_renderer->Load())
	{
		std::cout << "Error::Renderer::Load" << std::endl;
		return false;
	}

	return true;
}

/// <summary>
/// �������
/// </summary>
void GameMain::Delete()
{
	delete m_renderer;
	delete m_texturePool;
	delete m_meshPool;
	delete m_actorPool;

#ifdef _DEBUG

	delete m_debugger;

#endif

}

/// <summary>
/// ���̓v���Z�X�̍X�V����
/// </summary>
void GameMain::ProcessInput()
{
	// �I���C�x���g�̃L���b�`
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{

		switch (event.type)
		{
		case SDL_QUIT:
			m_isRunning = false;
			break;

		default:
			//CONTROLLER_INSTANCE.ReceiveEvent(event);
			break;
		}
	}

	// �L�[�{�[�h���͍X�V
	INPUT_KEYBOARD_INSTANCE.Update();

	// �R���g���[�����͍X�V
	//CONTROLLER_INSTANCE.Update();

	// �}�E�X���͍X�V
	//MOUSE_INSTANCE.Update();

	// ESC�������ꂽ��I��
	if (INPUT_KEYBOARD_INSTANCE.IsKeyPullUp(SDL_SCANCODE_ESCAPE))
	{
		m_isRunning = false;
	}


	// �A�N�^�[�f�o�b�O
	//if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_F12))
	//{
	//	ShowActor();
	//}

	// �����_�����O���\�[�X�\��
	//if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_F11))
	//{
	//	GetRenderer()->ShowResource();
	//}

	// �|�[�Y���[�h�ڍs�^����
	//if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_BACKSPACE) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_START))
	//{
	//	//�|�[�Y���[�h�ؑ�
	//	m_isPauseMode = !m_isPauseMode;
	//	Actor::State changeState;
	//	if (m_isPauseMode)
	//	{
	//		changeState = Actor::STATE_PAUSED;        // �|�[�Y
	//		m_pause->SetModeON();                     // �|�[�Y��ʂ��I��
	//	}
	//	else
	//	{
	//		m_pause->SetModeOFF();                    // �|�[�Y��ʂ��I�t
	//		changeState = Actor::STATE_ACTIVE;        // �A�N�e�B�u
	//	}

	//	//�S�ẴX�e�[�g��ύX����
	//	for (auto itr : m_actors)
	//	{
	//		if (itr->GetState() != Actor::STATE_DEAD)
	//		{
	//			itr->SetState(changeState);
	//		}
	//	}
	//}

#ifdef _DEBUG

	//------------------------------------------------+
	// �C�x���g�̍X�V
	//------------------------------------------------+

#endif
}

/// <summary>
/// �S�����̃��[�v�֐�
/// </summary>
/// <returns> ���[�v���Ŗ�肪���������ꍇfalse��Ԃ� </returns>
bool GameMain::RunLoop()
{
	//------------------------------------------------+
	// ���C�����[�v
	//------------------------------------------------+
	// ���͏���
	ProcessInput();

	// �f���^�^�C���X�V
	UpdateDeltaTime();

	// �V�[���̍X�V
	SceneBase* currentScene = m_scene->Update(m_deltaTime);
	// ���݃V�[�����X�V����Ă�����
	if (currentScene != m_scene)
	{
		// �O�̃V�[�����폜
		delete m_scene;
		// ���C���V�[�����X�V���ꂽ�V�[���ɐ؂�ւ��E������
		m_scene = currentScene;
		m_scene->Initialize();
	}

	// �A�N�^�[�̍X�V
	m_actorPool->UpdateObjects(m_deltaTime);

#ifdef _DEBUG

	//------------------------------------------------+
	// imgui�X�V�E�`�揈�� (Debug�r���h�ł̂ݍs��)
	//------------------------------------------------+
	m_debugger->UpdateImGui(m_deltaTime);

#endif

	//------------------------------------------------+
	// �`�揈��
	//------------------------------------------------+
	m_renderer->Draw();

#ifdef _DEBUG

	//------------------------------------------------+
	// imgui�`�揈�� (Debug�r���h�ł̂ݍs��)
	//------------------------------------------------+
	m_debugger->RenderImGui();

#endif
	


	return m_isRunning;
	
}


/// <summary>
/// �t���[���֌W�̍X�V����
/// </summary>
void GameMain::UpdateDeltaTime()
{
	// 16�~���b(= 60�t���[��/�b)�ɂȂ�悤�ɁA�O�̃t���[�������16�~���b�ȏ�o�߂���܂ő҂�
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_ticksCount + 16))
		;

	// �O�̃t���[�����猻�ݎ����܂ł̌o�ߎ��ԎZ�o(�b�P�ʁj
	m_deltaTime = (SDL_GetTicks() - m_ticksCount + 16) / 1000.0f;

	//���̃t���[���̊J�n���ԕۑ��i���̃t���[���܂ł̌o�ߎ��Ԃ��͂��邽�߁j
	m_ticksCount = SDL_GetTicks();

	// �t���[�����Ԃ����܂�ɂ��o�߂��Ă���ꍇ��0.05 �� 20fps�Œ�
	if (m_deltaTime > 0.05f)
	{
		m_deltaTime = 0.05f;
	}
}

/// <summary>
/// �V�[���̃Z�b�g
/// </summary>
/// <param name="_scene"> �V�[���x�[�X�N���X </param>
void GameMain::SetScene(SceneBase* _scene)
{
	if (m_scene == nullptr)
	{
		m_scene = _scene;
	}
	else
	{
		delete m_scene;
		m_scene = _scene;
	}

	m_scene->Initialize();
}