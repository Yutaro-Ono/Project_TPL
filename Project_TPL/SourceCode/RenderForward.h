#pragma once
#include "RenderMethodBase.h"


class RenderForward : public RenderMethodBase
{

public:

	RenderForward(class Renderer* _renderer);
	~RenderForward();

	bool Load() override;

	void Draw(class ShaderManager* _shaderManager, class DrawableObjectManager* _drawObjects) override;


private:

	bool CreateColorBuffer();


	// �V�F�[�_�[�N���X�|�C���^(�����E����̓V�F�[�_�[�}�l�[�W���[�N���X�ɔC����)
	class GLSLprogram* m_basicMeshShader;        // G�o�b�t�@�o�̓V�F�[�_�[(�W��)
	class GLSLprogram* m_phongShader;            // G�o�b�t�@�o�̓V�F�[�_�[(Phong�V�F�[�f�B���O)
	class GLSLprogram* m_normalMapShader;        // G�o�b�t�@�o�̓V�F�[�_�[(�m�[�}���}�b�s���O)
	class GLSLprogram* m_skyBoxShader;           // G�o�b�t�@�o�̓X�J�C�{�b�N�X�V�F�[�_�[
	class GLSLprogram* m_visualizeNormalShader;  // �@�������o������V�F�[�_�[
	class GLSLprogram* m_outScreenShader;        // ��ʏo�͗p�V�F�[�_�[

	// �t�H���[�h�����_�����O�o�͗p�o�b�t�@
	unsigned int m_colorFBO;
	unsigned int m_albedoSpec;
	unsigned int m_emissive;
	unsigned int m_attachments[2];
	unsigned int m_rbo;

};