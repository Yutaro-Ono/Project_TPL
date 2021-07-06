#include "RenderDeferred.h"
#include "GameMain.h"
#include "GameSettings.h"

RenderDeferred::RenderDeferred(class Renderer* _owner, RENDER_METHOD _method)
	:RenderMethodBase(_owner, _method)
{
}

RenderDeferred::~RenderDeferred()
{
}

bool RenderDeferred::Load()
{
	return (CreateGBuffer() && CreateLightBuffer()) == true;
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
