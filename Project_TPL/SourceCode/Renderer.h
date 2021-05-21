//----------------------------------------------------------------------------------+
// @file        Renderer.h
// @brief       �`�揈��
//              ���b�V���E�e�N�X�`���E�V�F�[�_�[���̕ێ�
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/22   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
// �C���N���[�h�t�@�C��
// ��gl3w��glfw�̏��ԂɋL�q
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include <vector>

class Renderer
{
public:

	Renderer();
	~Renderer();

	bool Initialize(int _width, int _height, bool _fullScreen);
	void Delete();

	void Draw();


	GLFWwindow* GetMainWindow() { return m_window; }    // �E�B���h�E�R���e�L�X�g�̃Q�b�^�[

	class DrawableObjectManager* GetDrawableObjectManager() { return m_drawableObject; }


	const glm::mat4 GetViewMatrix() { return m_viewMat; }
	const glm::mat4 GetProjectionMatrix() { return m_projMat; }

private:


	void SetUniformBuffer();

	static void FrameBuffer_Size_Callback(GLFWwindow* _window, int _width, int _height);


	GLFWwindow* m_window;                            // ���C���E�B���h�E

	class ShaderManager* m_shaderManager;            // �V�F�[�_�[�}�l�[�W���[�N���X
	class DrawableObjectManager* m_drawableObject;       // �`��\�I�u�W�F�N�g�Ǘ��N���X
	class BasicTriangle* m_triangle;                 // �f�o�b�O�p�O�p�`

	// �`��p�̍s��֘A
	glm::mat4 m_viewMat;                             // �r���[�s��
	glm::mat4 m_projMat;                             // �v���W�F�N�V�����s��

	// uniform�o�b�t�@
	unsigned int m_uboMatrices;                      // �r���[�E�v���W�F�N�V�����s��pUBO
	unsigned int m_uboCamera;                        // �J�������

};
