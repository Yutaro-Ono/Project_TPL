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

#include <iostream>

/// <summary>
/// �R���X�g���N�^
/// </summary>
GameMain::GameMain()
	:m_settings(nullptr)
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
	if (m_settings->GetInstance().Load("Project_TPL.ini"))
	{
		std::cout << "Error::GameSettings::Load" << std::endl;
		return false;
	}

	m_renderer = new Renderer();
	if (!m_renderer->Initialize())
	{
		std::cout << "Error::Renderer::Initialize" << std::endl;
		return false;
	}
	
#ifdef _DEBUG

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
	delete m_debugger;
}

/// <summary>
/// ���̓v���Z�X�̍X�V����
/// </summary>
void GameMain::ProcessInput()
{
	// ESCAPE�������ꂽ��
    // �E�B���h�E�I���t���O��ON����
	if (glfwGetKey(m_renderer->GetMainWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS || 
		glfwGetKey(m_debugger->GetDebugWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_debugger->GetDebugWindow(), true);
		glfwSetWindowShouldClose(m_renderer->GetMainWindow(), true);
		
	}
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
	// GL�C�x���g�̍X�V
	//------------------------------------------------+
	glfwPollEvents();          // �L�[�{�[�h�E�}�E�X�̃g���K�[���m�F

    // (GLFW�����炩�̑���ŕ���܂Ń��[�v)
	return !glfwWindowShouldClose(m_renderer->GetMainWindow());
}