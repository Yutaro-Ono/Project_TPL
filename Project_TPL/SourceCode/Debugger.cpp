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
#include "GameMain.h"
#include <iostream>

/// <summary>
/// �R���X�g���N�^
/// </summary>
Debugger::Debugger()
	:m_debugWindow(NULL)
	,m_windowH(1280)
	,m_windowW(720)
	,m_debugWidth(300)
	,m_debugHeight(300)
	,m_debugObjects(nullptr)
	,m_actorDebugObjects(nullptr)
{
	m_debugObjects = new DebugObjectPool();
	m_actorDebugObjects = new DebugObjectPool();
	CreateDebugBuffers();
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
	m_debugWindow = SDL_CreateWindow
	(
		"Debug_TPL",                                 // �E�B���h�E�̖���
		0,                                                // x���W�̃E�B���h�E�`�挴�_
		0,                                                // y���W�̃E�B���h�E�`�挴�_
		m_windowW,                                    // ��ʂ̉���
		m_windowH,                                   // ��ʂ̏c��
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	// �E�B���h�E�쐬���s��
	if (!m_debugWindow)
	{
		std::cout << "Failed : Create Debugger Window" << std::endl;
		return false;
	}

	// �f�o�b�O�E�B���h�E�ƃR���e�L�X�g�̃����N (�����_���[�Ƌ��ʂ̃R���e�L�X�g���g�p)
	SDL_GL_MakeCurrent(m_debugWindow, GAME_INSTANCE.GetRenderer()->GetContext());

	// SDL�����_���[�̍쐬
	m_debugRenderer = SDL_CreateRenderer(m_debugWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!m_debugRenderer)
	{
		std::cout << "Failed : Create Debugger Renderer" << std::endl;
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
	ImGui_ImplSDL2_InitForOpenGL(m_debugWindow, m_debugContext);
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
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	// SDL�̃N���[���A�b�v
	SDL_GL_DeleteContext(m_debugContext);
	SDL_DestroyWindow(m_debugWindow);
}

/// <summary>
/// ImGui�̍X�V����
/// �f�o�b�O�N���X�Q�̍X�V���s���\��
/// </summary>
void Debugger::UpdateImGui(float _deltaTime)
{

	// imgui�̐V�K�t���[�����J�n����
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_debugWindow);
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
	// �f�o�b�O�p�e�N�X�`���ւ̏������݂ƕ`��
	//m_debugObjects->RenderDebugBuffer();
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

}



/// <summary>
/// �`�揈��
/// �f�o�b�O�E�B���h�E�ɑ΂��ĕ`����s��
/// </summary>
void Debugger::RenderImGui()
{
	// �R���e�L�X�g�֘A�t�� (�����_���[�Ƌ���)
	SDL_GL_MakeCurrent(m_debugWindow, GAME_INSTANCE.GetRenderer()->GetContext());

	// �t���[���o�b�t�@�T�C�Y�̎擾
	SDL_GetWindowSize(m_debugWindow, &m_windowW, &m_windowH);
	// �r���[�|�[�g�̐ݒ�
	glViewport(0, 0, m_windowW, m_windowH);
	// �J���[�o�b�t�@���N���A
	glClear(GL_COLOR_BUFFER_BIT);

	// ImGui�����_�����O�J�n
	ImGui::Render();

	// �t���[���o�b�t�@��ImGui�̕`�挋�ʂ������o��
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// �V�����J���[�o�b�t�@���Â��o�b�t�@�ƌ������A��ʂɕ\��
	SDL_GL_SwapWindow(m_debugWindow);
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

/// <summary>
/// �f�o�b�O�p�t���[���o�b�t�@�̐�������
/// </summary>
/// <param name="_width"> ���� </param>
/// <param name="_height"> �c�� </param>
void Debugger::CreateDebugBuffers()
{

	// �J���[�o�b�t�@ (�e�N�X�`��)
	glGenTextures(1, &m_debugCB);
	glBindTexture(GL_TEXTURE_2D, m_debugCB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_debugWidth, m_debugHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// �����_�[�o�b�t�@
	glGenRenderbuffers(1, &m_debugRB);
	glBindRenderbuffer(GL_RENDERBUFFER, m_debugRB);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_debugWidth, m_debugHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// �t���[���o�b�t�@
	glGenFramebuffers(1, &m_debugFB);
	glBindFramebuffer(GL_FRAMEBUFFER, m_debugFB);
	// �e�N�X�`�����J���[�o�b�t�@�Ƃ��Č���
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_debugCB, 0);
	// �����_�[�o�b�t�@��[�x�o�b�t�@�Ƃ��Č���
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_debugRB);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


