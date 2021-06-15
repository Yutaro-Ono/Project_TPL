//----------------------------------------------------------------------------------+
// @file        Debugger.h
// @brief       �f�o�b�O�pUI(imgui���g�p)�̍X�V
//              �e��f�o�b�O�I�u�W�F�N�g�̕ێ�
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/24   �V�K�쐬
// 2021/ 3/24   �f�o�b�O��ʂ�ImGui���\�������悤�ɂ���
//----------------------------------------------------------------------------------+
#pragma once
// �C���N���[�h�t�@�C��
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include <vector>
#include "Renderer.h"
#include "Tag.h"



class Debugger
{

public:

	Debugger();
	~Debugger();

	bool Initialize();

	void Delete();

	void UpdateImGui(float _deltaTime);
	void RenderImGui();

	void AddDebugObject(class DebugObjectBase* _debugObj, OBJECT_TAG _tag);
	void DeleteDebugObject(class DebugObjectBase* _debugObj, OBJECT_TAG _tag);

	// �o�b�t�@���̎擾
	const unsigned int& GetDebugColorBuffer() const { return m_debugCB; }
	const unsigned int& GetDebugFrameBuffer() const { return m_debugFB; }
	// �o�b�t�@�c���T�C�Y�擾
	const int& GetDebugBufferWidth() const { return m_debugWidth; }
	const int& GetDebugBufferHeight() const { return m_debugHeight; }

	GLFWwindow* GetDebugWindow() { return m_debugWindow; }


private:

	void CreateDebugBuffers();

	
	GLFWwindow* m_debugWindow;            // �f�o�b�O�p�E�B���h�E

	class GLSLprogram* m_debugShader;     // �f�o�b�O�p�V�F�[�_�[
	unsigned int m_debugCB;               // �f�o�b�O�p�J���[�o�b�t�@
	unsigned int m_debugRB;               // �f�o�b�O�p�����_�[�o�b�t�@
	unsigned int m_debugFB;               // �f�o�b�O�p�t���[���o�b�t�@
	int m_debugWidth, m_debugHeight;

	int m_windowH;                        // �f�o�b�O�p�E�B���h�E�c�T�C�Y
	int m_windowW;                        // �f�o�b�O�p�E�B���h�E���T�C�Y

	class DebugObjectPool* m_debugObjects;
	class DebugObjectPool* m_actorDebugObjects;

};