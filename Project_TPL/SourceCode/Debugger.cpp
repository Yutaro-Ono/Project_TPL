//----------------------------------------------------------------------------------+
// @file        Debugger.cpp
// @brief       �f�o�b�O�pUI(imgui���g�p)�̍X�V
//              �e��f�o�b�O�I�u�W�F�N�g�̕ێ�
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//----------------------------------------------------------------------------------+
#include "Debugger.h"
#include "DebugObjectBase.h"
#include "DebugObjectPool.h"
#include <iostream>

/// <summary>
/// �R���X�g���N�^
/// </summary>
Debugger::Debugger()
	:m_debugWindow(NULL)
	,m_windowH(1280)
	,m_windowW(720)
	,m_debugObjects(nullptr)
	,m_actorDebugObjects(nullptr)
{
	m_debugObjects = new DebugObjectPool();
	m_actorDebugObjects = new DebugObjectPool();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Debugger::~Debugger()
{
	Delete();
}

/// <summary>
/// ����������
/// </summary>
/// <returns> ���������s����false��Ԃ� </returns>
bool Debugger::Initialize()
{

	//--------------------------------------+
    // �f�o�b�O�E�B���h�E��`
    //--------------------------------------+
    // �E�B���h�E�쐬
	m_debugWindow = glfwCreateWindow(m_windowH, m_windowW, "Debug_TPL", NULL, NULL);
	// �E�B���h�E�쐬���s��
	if (m_debugWindow == NULL)
	{
		std::cout << "Failed : Create Debugger Window" << std::endl;
		return false;
	}

	//----------------------------------------+
    // imgui�Z�b�g�A�b�v
    //----------------------------------------+
    // imgui�R���e�L�X�g�쐬
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// imgui�̃J���[�X�^�C����ݒ�
	ImGui::StyleColorsDark();         // �_�[�N�X�^�C��
	//ImGui::StyleColorsClassic();    // �N���V�b�N�X�^�C��
	// ���{��t�H���g�ɐݒ�
	io.Fonts->AddFontFromFileTTF("Data/Fonts/Debug/mplus-1c-medium.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

	// GL��GLSL�Ƀo�C���h
	// ver420���g�p���ł��A410 core �Ƃ��ăZ�b�g
	const char* glsl_version = "#version 410 core";
	ImGui_ImplGlfw_InitForOpenGL(m_debugWindow, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

    return true;
}

/// <summary>
/// �������
/// </summary>
void Debugger::Delete()
{
	// �f�o�b�O�I�u�W�F�N�g�̃v�[�������
	delete m_debugObjects;
	delete m_actorDebugObjects;

	// imgui�̃N���[���A�b�v
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	// window�̔j��
	glfwDestroyWindow(m_debugWindow);
}

/// <summary>
/// ImGui�̍X�V����
/// �f�o�b�O�N���X�Q�̍X�V���s���\��
/// </summary>
void Debugger::UpdateImGui(float _deltaTime)
{
	// �A�N�e�B�u�ȃE�B���h�E�Ƃ��ăf�o�b�O��ʂ�ݒ�
	glfwMakeContextCurrent(m_debugWindow);

	// imgui�̐V�K�t���[�����J�n����
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//-------------------------------------------------------------------------------+
	// �f�o�b�O�E�B���h�E 1 (�V�X�e���֘A)
	//-------------------------------------------------------------------------------+
	// �^�C�g���o�[�̃J���[�ݒ�(�I����)
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.7f, 0.0f, 0.7f, 1.0f));
	// �^�C�g���o�[�̃J���[�ݒ�(��I����)
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.4f, 0.0f, 0.4f, 1.0f));
	// �E�B���h�E�̃J���[�ݒ�
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.3f, 0.0f, 0.3f, 0.3f));
	// �E�B���h�E�̍��W�ݒ� (�ŏ��̈��̂�)
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	// �E�B���h�E�̃T�C�Y�ݒ�
	ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_Once);
	// �E�B���h�E�^�C�g��
	// �����{��t�H���g��u8���e�����Ő������\���ł���
	ImGui::Begin(u8"�f�o�b�O��� (�V�X�e��)");
	//�t���[�����[�g��\��
	ImGui::Text("Application average %.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Text("FrameRate >> %.1f FPS", ImGui::GetIO().Framerate);
	// �f�o�b�O�I�u�W�F�N�g�X�V����
	m_debugObjects->UpdateObjects(_deltaTime);
	// �V�X�e���E�B���h�E�I��
	ImGui::End();
	// �E�B���h�E�̃J���[�ݒ������
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	//-------------------------------------------------------------------------------+
	// �f�o�b�O�E�B���h�E 2 (�A�N�^�[�֘A)
	//-------------------------------------------------------------------------------+
	// �^�C�g���o�[�̃J���[�ݒ�(�I����)
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.2f, 0.7f, 1.0f));
	// �^�C�g���o�[�̃J���[�ݒ�(��I����)
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.1f, 0.4f, 1.0f));
	// �E�B���h�E�̃J���[�ݒ�
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.08f, 0.2f, 0.3f));
	// �E�B���h�E�̍��W�ݒ� (�ŏ��̈��̂�)
	ImGui::SetNextWindowPos(ImVec2(20, 300), ImGuiCond_Once);
	// �E�B���h�E�̃T�C�Y�ݒ�
	ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_Once);
	// �E�B���h�E�^�C�g��
	ImGui::Begin(u8"�f�o�b�O��� (�A�N�^�[)");
	// �f�o�b�O�I�u�W�F�N�g�X�V����
	m_actorDebugObjects->UpdateObjects(_deltaTime);
	// �A�N�^�[�E�B���h�E�I��
	ImGui::End();
	// �E�B���h�E�̃J���[�ݒ������
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	//-------------------------------------------------------------------------------+
	// �f�o�b�O�E�B���h�E 3 (�Q�[���ݒ�)
	//-------------------------------------------------------------------------------+
	// �^�C�g���o�[�̃J���[�ݒ�(�I����)
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	// �^�C�g���o�[�̃J���[�ݒ�(��I����)
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.4f, 0.1f, 1.0f));
	// �E�B���h�E�̃J���[�ݒ�
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.2f, 0.08f, 0.3f));
	// �E�B���h�E�̍��W�ݒ� (�ŏ��̈��̂�)
	ImGui::SetNextWindowPos(ImVec2(620, 20), ImGuiCond_Once);
	// �E�B���h�E�̃T�C�Y�ݒ�
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_Once);
	// ���j���[�^�u��������E�B���h�E�ɂ���
	ImGui::Begin(u8"�f�o�b�O��� (���ݒ�)", nullptr, ImGuiWindowFlags_MenuBar);
	// ���j���[�K�w����
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{

			}
			if (ImGui::MenuItem("Load"))
			{

			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	// �X�V����

	// �A�N�^�[�E�B���h�E�I��
	ImGui::End();
	// �E�B���h�E�̃J���[�ݒ������
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();


	// �`�揈��
	RenderImGui();

}

/// <summary>
/// �`�揈��
/// �f�o�b�O�E�B���h�E�ɑ΂��ĕ`����s��
/// </summary>
void Debugger::RenderImGui()
{
	// ImGui�����_�����O�J�n
	ImGui::Render();

	// �J���[�o�b�t�@���N���A
	glClear(GL_COLOR_BUFFER_BIT);

	// �t���[���o�b�t�@�T�C�Y�̎擾
	glfwGetFramebufferSize(m_debugWindow, &m_windowW, &m_windowH);
	// �r���[�|�[�g�̐ݒ�
	glViewport(0, 0, m_windowW, m_windowH);

	// �t���[���o�b�t�@��ImGui�̕`�挋�ʂ������o��
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	// �V�����J���[�o�b�t�@���Â��o�b�t�@�ƌ������A��ʂɕ\��
	glfwSwapBuffers(m_debugWindow);
}


void Debugger::AddDebugObject(DebugObjectBase* _debugObj, OBJECT_TAG _tag)
{

	if (_tag == OBJECT_TAG::SYSTEM)
	{
		m_debugObjects->AddObject(_debugObj);
	}
	else
	{
		m_actorDebugObjects->AddObject(_debugObj);
	}

	
}


void Debugger::DeleteDebugObject(DebugObjectBase* _debugObj, OBJECT_TAG _tag)
{
	if (_tag == OBJECT_TAG::SYSTEM)
	{
		m_debugObjects->DeleteObject(_debugObj);
	}
	else
	{
		m_actorDebugObjects->DeleteObject(_debugObj);
	}
}


