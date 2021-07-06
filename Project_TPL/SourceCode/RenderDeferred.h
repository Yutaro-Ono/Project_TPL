#pragma once
#include "RenderMethodBase.h"


class RenderDeferred : public RenderMethodBase
{
public:

	RenderDeferred(class Renderer* _renderer, RENDER_METHOD _method);
	~RenderDeferred();

	bool Load() override;

	void Draw(class DrawableObjectManager* _drawObjects) override;


private:

	bool CreateGBuffer();
	bool CreateLightBuffer();

	// �V�F�[�_�[�N���X�|�C���^(�����E����̓V�F�[�_�[�}�l�[�W���[�N���X�ɔC����)
	class GLSLprogram* m_basicMeshShader;    // G�o�b�t�@�o�̓V�F�[�_�[(�W��)
	class GLSLprogram* m_phongShader;         // G�o�b�t�@�o�̓V�F�[�_�[(Phong�V�F�[�f�B���O)
	class GLSLprogram* m_normalMapShader;     // G�o�b�t�@�o�̓V�F�[�_�[(�m�[�}���}�b�s���O)
	class GLSLprogram* m_skyBoxShader;         // G�o�b�t�@�o�̓X�J�C�{�b�N�X�V�F�[�_�[
	class GLSLprogram* m_dirLightShader;        // �f�B���N�V���i�����C�g�V�F�[�_�[
	class GLSLprogram* m_visualizeNormalShader;

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

};