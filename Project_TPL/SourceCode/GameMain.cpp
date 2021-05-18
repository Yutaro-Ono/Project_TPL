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
	:m_settings(nullptr)
	,m_texturePool(nullptr)
	,m_meshPool(nullptr)
	,m_renderer(nullptr)
	,m_debugger(nullptr)
	,m_scene(nullptr)
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
	if (!m_settings->GetInstance().Load("Project_TPL.ini"))
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
	if (!m_renderer->Initialize(m_settings->GetInstance().m_displayWidth, m_settings->GetInstance().m_displayHeight, m_settings->GetInstance().m_displayFullScreen))
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

	// ���C�����[�v
	//------------------------------------------------+
	// ���͂ɑ΂��鏈��
	//------------------------------------------------+
	ProcessInput();      // ���͂ɂ�鏈��

#ifdef _DEBUG

	//------------------------------------------------+
	// imgui�X�V�E�`�揈�� (Debug�r���h�ł̂ݍs��)
	//------------------------------------------------+
	m_debugger->UpdateImGui();

#endif

	//------------------------------------------------+
	// �`�揈��
	//------------------------------------------------+
	m_renderer->Draw();

	//------------------------------------------------+
	// GL�C�x���g�̍X�V
	//------------------------------------------------+
	glfwPollEvents();          // �L�[�{�[�h�E�}�E�X�̃g���K�[���m�F

    // (GLFW�����炩�̑���ŕ���܂Ń��[�v)
	return !glfwWindowShouldClose(m_renderer->GetMainWindow());
}