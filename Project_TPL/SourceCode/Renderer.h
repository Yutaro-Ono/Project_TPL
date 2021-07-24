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
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SDL.h>
#include <SDL_image.h>
#include <Effekseer.h>
#include <EffekseerRendererGL.h>
#include "Math.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include <unordered_map>
#include "RendererDebugObject.h"

// �`�惁�\�b�h
enum class RENDER_METHOD
{
	FORWARD = 0,
	DEFERRED,
	PBR,
	BLOOM
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


	SDL_Window* GetMainWindow() { return m_window; }    // �E�B���h�E�̃Q�b�^�[

	class DrawableObjectManager* GetDrawableObjectManager() { return m_drawableObject; }

	void SetRenderMethod(RENDER_METHOD _method) { m_renderMethodType = _method; }

	void BloomPass(unsigned int _highBrightBuffer, unsigned int _colorBuffer);

	const Matrix4& GetViewMatrix() const { return m_viewMat; }
	const Matrix4& GetProjectionMatrix() const { return m_projMat; }

	// Getter / Setter
	class RenderBloom* GetBloom() { return m_bloomRender; }
	class ShaderManager* GetShaderManager() { return m_shaderManager; }
	class DirectionalLight* GetDirectionalLight() { return m_dirLight; }
	class CubeMap* GetSkyBox() { return m_skyBox; }
	class VertexArray* GetQuadVertex() { return m_quadVA; }
	bool GetIsEnableVisualizeNormal() { return m_visualizeNormal; }
	bool GetIsEnableBloom() { return m_enableBloom; }

	void SetViewMatrix(const Matrix4& _viewMat) { m_viewMat = _viewMat; }

private:

	void CreateMSAA();

	void SetUniformBuffer();

	static void FrameBuffer_Size_Callback(SDL_Window* _window, int _width, int _height);


	SDL_Window* m_window;                            // ���C���E�B���h�E
	SDL_GLContext m_context;                         // �R���e�L�X�g
	SDL_Renderer* m_sdlRenderer;                     // SDL�����_���[


	RENDER_METHOD m_renderMethodType;                // �`����@

	class ShaderManager* m_shaderManager;            // �V�F�[�_�[�}�l�[�W���[�N���X
	class DrawableObjectManager* m_drawableObject;   // �`��\�I�u�W�F�N�g�Ǘ��N���X

	class RenderBloom* m_bloomRender;                // �u���[�����ʂ�t�^����N���X
	class DirectionalLight* m_dirLight;              // �A�N�e�B�u�ȃf�B���N�V���i�����C�g
	class CubeMap* m_skyBox;                         // skybox

	bool m_enableBloom;                              // �u���[���������邩���Ȃ���
	bool m_visualizeNormal;                          // �@�������o�����邩���Ȃ���

	// �e��`�惁�\�b�h�N���X
	std::unordered_map<RENDER_METHOD, class RenderMethodBase*> m_renderMethods;


	// �`��p�̍s��֘A
	Matrix4 m_viewMat;                             // �r���[�s��
	Matrix4 m_projMat;                             // �v���W�F�N�V�����s��

	// uniform�o�b�t�@
	unsigned int m_uboMatrices;                      // �r���[�E�v���W�F�N�V�����s��pUBO
	unsigned int m_uboCamera;                        // �J�������
	unsigned int m_uboTriggers;                      // �g���K�[��(�u���[�����ʂ̃I���I�t)
	unsigned int m_uboDirLights;                     // �f�B���N�V���i�����C�gUBO

	// vertexArray�N���X
	class VertexArray* m_quadVA;                     // �l�p�` (�X�N���[���Ɏg�p)

	// MSAA�o�b�t�@
	unsigned int m_msaaBuffer;
	unsigned int m_msaaColor;
	unsigned int m_msaaRBO;


	// �f�o�b�O�p�N���X
	class RendererDebugObject* m_debugObj;
	friend class RendererDebugObject;
};
