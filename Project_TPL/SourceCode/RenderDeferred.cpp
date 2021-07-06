#include "RenderDeferred.h"
#include "GameMain.h"
#include "Renderer.h"
#include "GameSettings.h"
#include "ShaderManager.h"
#include "CubeMap.h"
#include "VertexArray.h"
#include "DrawableObjectManager.h"
#include "RenderBloom.h"
#include "DirectionalLight.h"

RenderDeferred::RenderDeferred(class Renderer* _renderer)
	:RenderMethodBase(_renderer)
	,m_basicMeshShader(_renderer->GetShaderManager()->GetShader(GLSLshader::GBUFFER_BASIC_MESH))
	,m_phongShader(_renderer->GetShaderManager()->GetShader(GLSLshader::GBUFFER_PHONG))
	,m_normalMapShader(_renderer->GetShaderManager()->GetShader(GLSLshader::GBUFFER_NORMALMAP))
	,m_skyBoxShader(_renderer->GetShaderManager()->GetShader(GLSLshader::GBUFFER_BASIC_SKYBOX))
	,m_dirLightShader(_renderer->GetShaderManager()->GetShader(GLSLshader::DIRECTIONAL_LIGHT_PASS))
	,m_visualizeNormalShader(_renderer->GetShaderManager()->GetShader(GLSLshader::OPTION_NORMAL_VISUALIZE_GBUFFER))
	,m_outScreenShader(_renderer->GetShaderManager()->GetShader(GLSLshader::OUT_SCREEN_ENTIRE))
{
}

RenderDeferred::~RenderDeferred()
{
}

bool RenderDeferred::Load()
{
	bool success = true;
	success = CreateGBuffer();
	success = CreateLightBuffer();

	return success;
}

void RenderDeferred::Draw(class ShaderManager* _shaderManager, class DrawableObjectManager* _drawObjects)
{
	// G�o�b�t�@���o�C���h
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

	// �J���[�E�o�b�t�@���̃N���A
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);      // �w�肵���F�l�ŉ�ʂ��N���A
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);              // ��ʂ̃J���[�E�[�x�E�X�e���V���o�b�t�@���N���A
	glEnable(GL_DEPTH_TEST);

	//-------------------------------------------------------------------------
	// 0.�W�I���g���p�X
	//-------------------------------------------------------------------------
	// SkyBox
	m_skyBoxShader->UseProgram();
	glm::mat4 remView = glm::mat4(glm::mat3(m_renderer->GetViewMatrix()));
	m_skyBoxShader->SetUniform("u_removeTransView", remView);
	m_renderer->GetSkyBox()->Draw(m_skyBoxShader);

	// Mesh
	glEnable(GL_DEPTH_TEST);
	m_basicMeshShader->UseProgram();
	m_basicMeshShader->SetUniform("u_mat.albedo", 0);
	m_basicMeshShader->SetUniform("u_mat.specular", 5);
	m_basicMeshShader->SetUniform("u_mat.emissive", 6);
	_drawObjects->Draw(m_basicMeshShader);

	// NormalMapping
	//glEnable(GL_DEPTH_TEST);
	//m_normalMapShader->UseProgram();
	//m_normalMapShader->SetUniform("u_mat.albedo", 0);
	//m_normalMapShader->SetUniform("u_mat.normal", 2);
	//m_normalMapShader->SetUniform("u_mat.specular", 5);
	//m_normalMapShader->SetUniform("u_mat.emissive", 6);
	//m_normalMapShader->SetUniform("u_lightPos", m_renderer->GetDirectionalLight()->GetPosition());
	//_drawObjects->Draw(m_normalMapShader);

	// Phong�V�F�[�f�B���O
	//m_phongShader->UseProgram();
	//m_phongShader->SetUniform("u_mat.albedo", 0);
	//m_phongShader->SetUniform("u_mat.specular", 5);
	//m_phongShader->SetUniform("u_mat.emissive", 6);
	//m_drawableObject->Draw(m_shaderManager, GLSLshader::GBUFFER_PHONG);

	// �@���̎��o��
	if (m_renderer->GetIsEnableVisualizeNormal())
	{
		glEnable(GL_DEPTH_TEST);
		m_visualizeNormalShader->UseProgram();
		_drawObjects->Draw(m_visualizeNormalShader);
	}

	// G�o�b�t�@�o�C���h����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//----------------------------------------------------------------------------+
	// ���C�e�B���O�p�X
	//----------------------------------------------------------------------------+
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightBuffer);
	// �u�����h�̗L����
	glEnablei(GL_BLEND, 0);
	glBlendFuncSeparatei(0, GL_ONE, GL_ONE, GL_ONE, GL_ONE);     // ���Z����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	// �J�����O�̗L����
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);
	// GBuffer���e�N�X�`���Ƃ��ăo�C���h
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_gPos);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_gEmissive);

	//-------------------------------------------------------------------+
	// Point Light

	//-------------------------------------------------------------------+
	// Spot Light


	glDisable(GL_CULL_FACE);

	//-------------------------------------------------------------------+
	// Directional Light
	m_dirLightShader->UseProgram();
	m_dirLightShader->SetUniform("u_gBuffer.position", 0);
	m_dirLightShader->SetUniform("u_gBuffer.normal", 1);
	m_dirLightShader->SetUniform("u_gBuffer.albedoSpec", 2);
	m_dirLightShader->SetUniform("u_gBuffer.emissive", 3);
	m_renderer->GetQuadVertex()->SetActive();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// ���C�g�p�X��̓u�����h��؂�
	glDisable(GL_BLEND);
	glDisablei(GL_BLEND, 0);

	// gBuffer�̐[�x�������C�g�o�b�t�@�փR�s�[����
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer);            // �ǂݎ���Ƃ���GBuffer���w��
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_lightBuffer);        // �������ݐ�Ƀ��C�g�o�b�t�@���w��

	glBlitFramebuffer(0, 0, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(),
		0, 0, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(),
		GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, m_lightBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);


	// �u���[�����ʂ��L���łȂ��ꍇ
	// G�o�b�t�@+���C�g�p�X�̌��ʂ����̂܂܉�ʂ֏o�͂���
	if (m_renderer->GetIsEnableBloom())
	{
		m_renderer->BloomPass(m_lightHighBright, m_lightHDR);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_outScreenShader->UseProgram();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_lightHDR);

		// �X�N���[����`��
		m_renderer->GetQuadVertex()->SetActive();
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}




/// <summary>
/// G�o�b�t�@�̐���
/// </summary>
/// <returns> �t���[���o�b�t�@������ɐ�������Ă����true </returns>
bool RenderDeferred::CreateGBuffer()
{
	// G�o�b�t�@�̐����E�o�C���h
	glGenFramebuffers(1, &m_gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

	//--------------------------------------------------------------------+
	// G�o�b�t�@�e�v�f�̓o�^
	//--------------------------------------------------------------------+
	// ���W���o�b�t�@ (���������_�o�b�t�@ / 0�Ԗ�)
	glGenTextures(1, &m_gPos);
	glBindTexture(GL_TEXTURE_2D, m_gPos);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gPos, 0);
	// �@�����o�b�t�@ (���������_�o�b�t�@ / 1�Ԗ�)
	glGenTextures(1, &m_gNormal);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gNormal, 0);
	// �A���x�h&�X�y�L�������o�b�t�@ (8bit�J���[�o�b�t�@ / 2�Ԗ�)
	glGenTextures(1, &m_gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gAlbedoSpec, 0);
	// ���P�x�o�b�t�@ (�G�~�b�V�u�o�b�t�@ / 3�Ԗ�)
	glGenTextures(1, &m_gEmissive);
	glBindTexture(GL_TEXTURE_2D, m_gEmissive);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_gEmissive, 0);

	// ���������e�v�f��G�o�b�t�@�̕`���Ƃ���GL�ɖ�������
	m_gAttachments[0] = { GL_COLOR_ATTACHMENT0 };
	m_gAttachments[1] = { GL_COLOR_ATTACHMENT1 };
	m_gAttachments[2] = { GL_COLOR_ATTACHMENT2 };
	m_gAttachments[3] = { GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, m_gAttachments);

	// �����_�[�o�b�t�@�̐����E�o�^
	glGenRenderbuffers(1, &m_gRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_gRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_gRBO);

	// �t���[���o�b�t�@�̐������`�F�b�N
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::GBuffer::Create Failed" << std::endl;
		return false;
	}

	// G�o�b�t�@�̃o�C���h������
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

/// <summary>
/// ���C�g�o�b�t�@�̐���
/// </summary>
/// <returns> ���C�g�o�b�t�@������ɐ�������Ă����true </returns>
bool RenderDeferred::CreateLightBuffer()
{
	glGenFramebuffers(1, &m_lightBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightBuffer);

	// HDR�o�b�t�@
	glGenTextures(1, &m_lightHDR);
	glBindTexture(GL_TEXTURE_2D, m_lightHDR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_lightHDR, 0);
	// ���P�x�o�b�t�@�̍쐬
	glGenTextures(1, &m_lightHighBright);
	glBindTexture(GL_TEXTURE_2D, m_lightHighBright);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_lightHighBright, 0);
	// �A�^�b�`�����g�ݒ�
	m_lightAttachments[0] = { GL_COLOR_ATTACHMENT0 };
	m_lightAttachments[1] = { GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, m_lightAttachments);

	// �����_�[�o�b�t�@���쐬����
	glGenRenderbuffers(1, &m_lightRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_lightRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_lightRBO);

	// �t���[���o�b�t�@�̐��������`�F�b�N
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::LightBuffer::Create Failed" << std::endl;
		return false;
	}
	// �t���[���o�b�t�@�̃o�C���h����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}
