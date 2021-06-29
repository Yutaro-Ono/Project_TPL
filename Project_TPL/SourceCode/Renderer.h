//----------------------------------------------------------------------------------+
// @file        Renderer.h
// @brief       �`�揈��
//              ���b�V���E�e�N�X�`���E�V�F�[�_�[���̕ێ�
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/22   �V�K�쐬
// 2021/ 5/26   �e��o�b�t�@��o�^ (G�o�b�t�@�E���C�g�EMSAA)
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
#include "RendererDebugObject.h"

// �`�惁�\�b�h
enum class RENDER_METHOD
{
	FORWARD = 0,
	DEFFERED
};


class Renderer
{
public:

	Renderer();
	~Renderer();

	bool Initialize(int _width, int _height, bool _fullScreen);
	bool Load();
	void CreateRendererDebugObject();
	void Delete();

	void Draw();


	GLFWwindow* GetMainWindow() { return m_window; }    // �E�B���h�E�R���e�L�X�g�̃Q�b�^�[

	class DrawableObjectManager* GetDrawableObjectManager() { return m_drawableObject; }

	void SetRenderMethod(RENDER_METHOD _method) { m_renderMethod = _method; }

	const glm::mat4 GetViewMatrix() { return m_viewMat; }
	const glm::mat4 GetProjectionMatrix() { return m_projMat; }

	class DirectionalLight* GetDirectionalLight() { return m_dirLight; }

	void SetViewMatrix(const glm::mat4& _viewMat) { m_viewMat = _viewMat; }

private:

	void CreateGBuffer();
	void CreateLightBuffer();
	void CreateMSAA();

	void SetUniformBuffer();

	static void FrameBuffer_Size_Callback(GLFWwindow* _window, int _width, int _height);


	GLFWwindow* m_window;                            // ���C���E�B���h�E

	RENDER_METHOD m_renderMethod;                    // �`����@

	class ShaderManager* m_shaderManager;            // �V�F�[�_�[�}�l�[�W���[�N���X
	class DrawableObjectManager* m_drawableObject;   // �`��\�I�u�W�F�N�g�Ǘ��N���X

	class RenderBloom* m_bloomRender;                // �u���[�����ʂ�t�^����N���X
	class DirectionalLight* m_dirLight;              // �A�N�e�B�u�ȃf�B���N�V���i�����C�g
	class CubeMap* m_skyBox;                         // skybox

	bool m_enableBloom;                              // �u���[���������邩���Ȃ���
	bool m_visualizeNormal;                          // �@�������o�����邩���Ȃ���

	// �`��p�̍s��֘A
	glm::mat4 m_viewMat;                             // �r���[�s��
	glm::mat4 m_projMat;                             // �v���W�F�N�V�����s��

	// uniform�o�b�t�@
	unsigned int m_uboMatrices;                      // �r���[�E�v���W�F�N�V�����s��pUBO
	unsigned int m_uboCamera;                        // �J�������
	unsigned int m_uboTriggers;                      // �g���K�[��(�u���[�����ʂ̃I���I�t)
	unsigned int m_uboDirLights;                     // �f�B���N�V���i�����C�gUBO

	// vertexArray�N���X
	class VertexArray* m_quadVA;                     // �l�p�` (�X�N���[���Ɏg�p)

	// G�o�b�t�@
	unsigned int m_gBuffer;
	// G�o�b�t�@�v�f
	unsigned int m_gPos;
	unsigned int m_gNormal;
	unsigned int m_gAlbedoSpec;
	unsigned int m_gEmissive;
	unsigned int m_gAttachments[4];
	unsigned int m_gRBO;

	// ���C�g�o�b�t�@
	unsigned int m_lightBuffer;
	// ���C�g�o�b�t�@�v�f
	unsigned int m_lightHDR;
	unsigned int m_lightHighBright;
	unsigned int m_lightRBO;
	unsigned int m_lightAttachments[2];

	// MSAA�o�b�t�@
	unsigned int m_msaaBuffer;
	unsigned int m_msaaColor;
	unsigned int m_msaaRBO;


	// �f�o�b�O�p�N���X
	class RendererDebugObject* m_debugObj;
	friend class RendererDebugObject;
};
