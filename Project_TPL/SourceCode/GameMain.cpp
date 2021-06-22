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

#include <iostream>



/// <summary>
/// �R���X�g���N�^
/// </summary>
GameMain::GameMain()
	:m_texturePool(nullptr)
	,m_meshPool(nullptr)
	,m_actorPool(nullptr)
	,m_renderer(nullptr)
	,m_debugger(nullptr)
	,m_scene(nullptr)
	,m_deltaTime(1.0f)
	,m_currentFrame(glfwGetTime() / 1000.0f)
	,m_lastFrame(glfwGetTime() / 1000.0f)
	,m_bulletTime(1.0f)
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
	// �����_���[�p�f�o�b�O�I�u�W�F�N�g�̐���
	m_renderer->CreateRendererDebugObject();

#endif

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
	// ESCAPE�������ꂽ��
    // �E�B���h�E�I���t���O��ON����
	if (glfwGetKey(m_renderer->GetMainWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_renderer->GetMainWindow(), true);
	}

#ifdef _DEBUG

	// �f�o�b�O���
	if (glfwGetKey(m_debugger->GetDebugWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_debugger->GetDebugWindow(), true);
	}

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
	

	//------------------------------------------------+
	// GL�C�x���g�̍X�V
	//------------------------------------------------+
	glfwPollEvents();          // �L�[�{�[�h�E�}�E�X�̃g���K�[���m�F

    // (GLFW�����炩�̑���ŕ���܂Ń��[�v)
	return !glfwWindowShouldClose(m_renderer->GetMainWindow());
}


/// <summary>
/// �t���[���֌W�̍X�V����
/// </summary>
void GameMain::UpdateDeltaTime()
{
	bool updatePermit = false;
	
	// ���݂̃t���[�����擾
	m_currentFrame = glfwGetTime() / 1000.0f;

	if (m_currentFrame >= m_lastFrame + (16.0f / 1000.0f))
	{
		updatePermit = true;
	}

	if (updatePermit)
	{
		// �O�t���[�����猻�݃t���[���܂ł̌o�ߎ��Ԃ��Z�o(�b�P��)
		m_deltaTime = ((glfwGetTime() / 1000.0f) - m_lastFrame + (16.0f / 1000.0f));
	}

	// �t���[�����X�V
	m_lastFrame = glfwGetTime() / 1000.0f;

	// �t���[�����Ԃ��o�߂������Ă���ꍇ��0.05 �� 20fps�Œ�
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