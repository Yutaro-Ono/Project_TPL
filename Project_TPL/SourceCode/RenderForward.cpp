#include "RenderForward.h"
#include "GameMain.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "GameSettings.h"
#include "ShaderManager.h"
#include "DrawableObjectManager.h"

RenderForward::RenderForward(Renderer* _renderer)
	:RenderMethodBase(_renderer)
{
}

RenderForward::~RenderForward()
{
}

bool RenderForward::Load()
{
	return CreateColorBuffer();
}

void RenderForward::Draw(ShaderManager* _shaderManager, DrawableObjectManager* _drawObjects)
{

	glBindFramebuffer(GL_FRAMEBUFFER, m_colorFBO);

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);      // �w�肵���F�l�ŉ�ʂ��N���A
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);              // ��ʂ̃J���[�E�[�x�E�X�e���V���o�b�t�@���N���A

    // ���b�V���̕`��
    glEnable(GL_DEPTH_TEST);
    _shaderManager->EnableShaderProgram(GLSLshader::BASIC_MESH);
    _drawObjects->Draw(_shaderManager->GetShader(GLSLshader::BASIC_MESH));

    // �@���̎��o��
    if (m_renderer->GetIsEnableVisualizeNormal())
    {
        _shaderManager->EnableShaderProgram(GLSLshader::OPTION_NORMAL_VISUALIZE);
        _drawObjects->Draw(_shaderManager->GetShader(GLSLshader::OPTION_NORMAL_VISUALIZE));
    }

	// �u���[�����ʂ��L���łȂ��ꍇ
    // G�o�b�t�@+���C�g�p�X�̌��ʂ����̂܂܉�ʂ֏o�͂���
	if (m_renderer->GetIsEnableBloom())
	{
		m_renderer->BloomPass(m_emissive, m_albedoSpec);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_outScreenShader->UseProgram();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_albedoSpec);

		// �X�N���[���֕`��
		m_renderer->GetQuadVertex()->SetActive();
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


/// <summary>
/// �t�H���[�h�����_�����O�p�J���[�o�b�t�@�̐���
/// </summary>
bool RenderForward::CreateColorBuffer()
{
	// G�o�b�t�@�̐����E�o�C���h
	glGenFramebuffers(1, &m_colorFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_colorFBO);

	//--------------------------------------------------------------------+
	// �J���[�o�b�t�@�e�v�f�̓o�^
	//--------------------------------------------------------------------+
	// �A���x�h&�X�y�L�������o�b�t�@ (8bit�J���[�o�b�t�@ / 0�Ԗ�)
	glGenTextures(1, &m_albedoSpec);
	glBindTexture(GL_TEXTURE_2D, m_albedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_albedoSpec, 0);
	// ���P�x�o�b�t�@ (�G�~�b�V�u�o�b�t�@ / 1�Ԗ�)
	glGenTextures(1, &m_emissive);
	glBindTexture(GL_TEXTURE_2D, m_emissive);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_emissive, 0);

	// ���������e�v�f��G�o�b�t�@�̕`���Ƃ���GL�ɖ�������
	m_attachments[0] = { GL_COLOR_ATTACHMENT0 };
	m_attachments[1] = { GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, m_attachments);

	// �����_�[�o�b�t�@�̐����E�o�^
	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

	// �t���[���o�b�t�@�̐������`�F�b�N
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::ColorBuffer *ForwardRender::Create Failed" << std::endl;
		return false;
	}

	// �o�C���h������
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}
