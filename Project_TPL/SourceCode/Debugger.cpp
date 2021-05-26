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
	,m_windowH(640)
	,m_windowW(480)
	,m_debugObjectPool(nullptr)
{
	m_debugObjectPool = new DebugObjectPool();
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
    // �E�B���h�E�쐬 (1920x1080)
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
	delete m_debugObjectPool;

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

	// �E�B���h�E�^�C�g��
	// �����{��t�H���g��u8���e�����Ő������\���ł���
	ImGui::Begin("Debug Screen");

	// �e�L�X�g�\��
	//ImGui::Text("index");

	// �e�L�X�g�\�� (�����I�ɉ��s�����)
	//ImGui::Text("index");

	// ���s���Ȃ��Ƃ��������B���̃e�L�X�g�������s�ɕ\�������
	//ImGui::SameLine();

	// �e�L�X�g
	//ImGui::Text(u8"�ł��B");

	// ��؂��
	//ImGui::Separator();

	// �`�F�b�N�{�b�N�X
	//static bool check;
	//ImGui::Checkbox(u8"�`�F�b�N", &check);

	// ��؂��
	//ImGui::Separator();

	// �J���[�s�b�J�[
	//static float color;
	//ImGui::ColorPicker4(u8"�J���[", &color);

	//�t���[�����[�g��\��
	ImGui::Text("Application average %.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Text("FrameRate >> %.1f FPS", ImGui::GetIO().Framerate);

	// �f�o�b�O�I�u�W�F�N�g�v�[���̍X�V����
	m_debugObjectPool->UpdateObjects(_deltaTime);


	ImGui::End();

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


void Debugger::AddDebugObject(DebugObjectBase* _debugObj)
{
	m_debugObjectPool->AddObject(_debugObj);
}


void Debugger::DeleteDebugObject(DebugObjectBase* _debugObj)
{
	m_debugObjectPool->DeleteObject(_debugObj);
}


