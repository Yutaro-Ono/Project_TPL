//----------------------------------------------------------------------------------+
// @file        RenderBloom.cpp
// @brief       �u���[���`�揈��
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 6/04   �V�K�쐬
//----------------------------------------------------------------------------------+
#include "RenderBloom.h"
#include "Renderer.h"
#include "GameMain.h"
#include "GameSettings.h"
#include "GLSLprogram.h"
#include "VertexArray.h"

// �k���o�b�t�@�̒i�K�� (1/2, 1/4, 1/8, 1/16, 1/32��5�i�K)
const unsigned int maxLevelNum = 5;

RenderBloom::RenderBloom()
	:m_exposure(1.0f)
	,m_gamma(2.2f)
{

}

RenderBloom::~RenderBloom()
{
}

/// <summary>
/// Step 1 :
/// ���P�x�o�b�t�@���k���o�b�t�@�ɒi�K�ۑ�����
/// </summary>
/// <param name="_brightBuffer"> �k���ۑ����鍂�P�x�o�b�t�@ </param>
/// <param name="_downSampleShader"> �_�E���T���v�����O�p�V�F�[�_�[ </param>
/// <param name="_screenVA"> �X�N���[���`��p���_�z�� </param>
void RenderBloom::DownSampling(unsigned int _brightBuffer, GLSLprogram* _downSampleShader, VertexArray* _screenVA)
{
	// �[�x�e�X�g���I�t
	glDisable(GL_DEPTH_TEST);

	// �k���o�b�t�@�ɕ`�悷��^�[�Q�b�g
	unsigned int renderSource = _brightBuffer;

	int reduceX = GAME_CONFIG.GetScreenSizeW();
	int reduceY = GAME_CONFIG.GetScreenSizeH();

	// �k���o�b�t�@�̊�ԖڂɃ_�E���T���v�����O���ʂ��o�͂���
	for (int i = 0; i < maxLevelNum; i++)
	{
		// �k���o�b�t�@�̏c���T�C�Y���X�V
        // 1/2, 1/4, 1/8, 1/16, 1/32...
		reduceX /= 2;
		reduceY /= 2;

		// �k���o�b�t�@�Ɋ�Ԗڂ��w��
		glBindFramebuffer(GL_FRAMEBUFFER, m_blurFBOs[i * 2 + 1]);
		// Viewport�̒���
		glViewport(0, 0, reduceX, reduceY);
		// �o�b�t�@�̃N���A
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// �^�[�Q�b�g�������P�x�o�b�t�@���V�F�[�_�[���Ń_�E���T���v�����O���o��
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, renderSource);
		_downSampleShader->UseProgram();
		_downSampleShader->SetUniform("u_screenTexture", 0);

		// �X�N���[���Ƃ��ĕ`��
		_screenVA->SetActive();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// �k���o�b�t�@�Ƀ^�[�Q�b�g���X�V
		renderSource = m_blurTextures[i * 2 + 1];
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

/// <summary>
/// Step 2 :
/// �k���o�b�t�@�ɃK�E�X�ڂ������{��
/// </summary>
/// <param name="_brightBuffer"> ���P�x�o�b�t�@ </param>
/// <param name="_gaussShader"> �K�E�X�ڂ����p�V�F�[�_�[ </param>
/// <param name="_screenVA"> �X�N���[���`��p���_�z�� </param>
void RenderBloom::GaussBlur(unsigned int _brightBuffer, GLSLprogram* _gaussShader, VertexArray* _screenVA)
{
	const int sampleCount = 15;
	glm::vec3 offset[sampleCount];

	int reduceX = GAME_CONFIG.GetScreenSizeW();
	int reduceY = GAME_CONFIG.GetScreenSizeH();
	float deviation = 2.0f;

	// �^�[�Q�b�g����k���o�b�t�@
	unsigned int renderSource = m_blurTextures[1];

	// �K�E�X�ڂ����i�K�������[�v (5�i�K)
	for (int i = 0; i < maxLevelNum; i++)
	{
		// �k���o�b�t�@�̏c���T�C�Y���X�V
		reduceX /= 2;
		reduceY /= 2;

		bool h = false;
		// horizontal : 0 = ����, 1 = ���������ɃK�E�X�ڂ�����������
		for (int horizontal = 0; horizontal < 2; horizontal++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_blurFBOs[i * 2 + horizontal]);
			// �r���[�|�[�g�̍X�V
			glViewport(0, 0, reduceX, reduceY);
			glm::vec2 dir(1.0f - (float)horizontal, (float)horizontal);

			// �K�E�X�ڂ����v�Z
			CalcGaussBlurParam(reduceX, reduceY, dir, deviation, offset);

			// �J���[�o�b�t�@�̃N���A
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// �K�E�X�ڂ����������{���ĕ`��
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, renderSource);
			_gaussShader->UseProgram();
			_gaussShader->SetUniform("u_blurSource", 0);
			_gaussShader->SetUniform("u_param.sampleCount", sampleCount);

			// �V�F�[�_�[�ɃI�t�Z�b�g���𑗐M
			for (int i = 0; i < sampleCount; i++)
			{
				std::string uStr = "u_param.offset[" + std::to_string(i) + "]";
				_gaussShader->SetUniform(uStr.c_str(), offset[i]);
			}
			// �X�N���[���ɕ`��
			_screenVA->SetActive();
			glDrawArrays(GL_TRIANGLES, 0, 6);

			// �^�[�Q�b�g�ɂ���o�b�t�@���X�V
			renderSource = m_blurTextures[i * 2 + horizontal];
		}
		// ���[�v�����Ƃɕ΍����C���N�������g
		deviation += 1.5f;
	}

	// �o�C���h�����E�r���[�|�[�g��ʏ�X�N���[���T�C�Y�ɕύX
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH());

}

/// <summary>
/// Step3 :
/// �ŏI�o�͉�ʂɃu���[�����ʂ���������
/// </summary>
/// <param name="_blendBuffer"> �u���[�������������`��o�b�t�@ </param>
/// <param name="_bloomShader"> �u���[�����ʃV�F�[�_�[ </param>
/// <param name="_screenVA"> �X�N���[���`��p���_�z�� </param>
void RenderBloom::DrawBlendBloom(unsigned int _blendBuffer, GLSLprogram* _bloomShader, VertexArray* _screenVA)
{
	// �[�x�e�X�g���I�t
	glDisable(GL_DEPTH_TEST);
	// �o�b�t�@�̃J���[�v�f���N���A
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// �����������o�b�t�@���T���v�����O����
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _blendBuffer);
	_bloomShader->UseProgram();
	_bloomShader->SetUniform("u_exposure", m_exposure);
	_bloomShader->SetUniform("u_gamma", m_gamma);
	_bloomShader->SetUniform("u_scene", 0);
	// �k���o�b�t�@�̒i�K���Ƀu���[�����ʂ�������
	for (unsigned int i = 0; i < maxLevelNum; i++)
	{
		int num = i + 1;
		std::string str = "u_bloom" + std::to_string(num);
		glActiveTexture(GL_TEXTURE0 + num);
		glBindTexture(GL_TEXTURE_2D, m_blurTextures[i * 2 + 1]);
		_bloomShader->SetUniform(str.c_str(), num);
	}

	// �X�N���[���ɕ`��
	_screenVA->SetActive();
	glDrawArrays(GL_TRIANGLES, 0, 6);

}

/// <summary>
/// �_�E���T���v�����O�E�ڂ����p�̃t���[���o�b�t�@���쐬����@(�쐣���u���[���t�B���^���̗p)
/// �K�E�X�ڂ����ɂďc��2�����g�p����̂ŁA�i�K���Ƃ�2�����쐬 (�v5�i�K x 2)
/// </summary>
void RenderBloom::CreateBlurFBO()
{
	// �X�N���[���̏c���T�C�Y (=�e�N�X�`���̏c���T�C�Y)
	int w = GAME_CONFIG.GetScreenSizeW();
	int h = GAME_CONFIG.GetScreenSizeH();

	// �i�K�������[�v
	for (unsigned int i = 0; i < maxLevelNum; i++)
	{
		// �k���o�b�t�@�̏c���T�C�Y���X�V
		// 1/2, 1/4, 1/8, 1/16, 1/32...
		w /= 2;
		h /= 2;

		// �k���o�b�t�@�ƃe�N�X�`��ID�̗̈���m�ۂ���
        // �k���o�b�t�@5�i�K x �c��2��
		if (m_blurFBOs.empty())
		{
			m_blurFBOs.resize(maxLevelNum * 2);
		}
		if (m_blurTextures.empty())
		{
			m_blurTextures.resize(maxLevelNum * 2);
		}

		// �e�i�K���Ƃɏc��2�������[�v (0 = ��, 1 = �c)
		for (unsigned int j = 0; j < 2; j++)
		{

			// �k���o�b�t�@�e�N�X�`���̍쐬
			glGenTextures(1, &m_blurFBOs[i * 2 + j]);
			glBindTexture(GL_TEXTURE_2D, m_blurTextures[i * 2 + 1]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			// �k���o�b�t�@�̍쐬
			glGenFramebuffers(1, &m_blurFBOs[i * 2 + j]);
			glBindFramebuffer(GL_FRAMEBUFFER, m_blurFBOs[i * 2 + j]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_blurTextures[i * 2 + j], 0);

			// �o�C���h����
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

	}

}

/// <summary>
/// �K�E�X�ڂ����v�Z
/// </summary>
/// <param name="_w"></param>
/// <param name="_h"></param>
/// <param name="_dir"></param>
/// <param name="_deviation"></param>
/// <param name="_offset"></param>
void RenderBloom::CalcGaussBlurParam(int _w, int _h, const glm::vec2& _dir, float _deviation, glm::vec3* _offset)
{
	float tu = 1.0f / float(_w);
	float tv = 1.0f / float(_h);
	_offset[0].x = 0.0f;
	_offset[0].y = 0.0f;
	_offset[0].z = GaussianDistribution(glm::vec2(0.0f, 0.0f), _deviation);
	float totalWeight = _offset[0].z;

	for (auto i = 0; i < 8; ++i)
	{
		int nextPos = (i - 1) * 2 + 1;
		_offset[i].x = _dir.x * tu * nextPos;
		_offset[i].y = _dir.y * tv * nextPos;
		_offset[i].z = GaussianDistribution(_dir * float(nextPos), _deviation);
		totalWeight += _offset[i].z * 2.0f;
	}
	for (auto i = 0; i < 8; ++i)
	{
		_offset[i].z /= totalWeight;
	}
	for (auto i = 8; i < 15; ++i)
	{
		_offset[i].x = -_offset[i - 7].x;
		_offset[i].y = -_offset[i - 7].y;
		_offset[i].z = _offset[i - 7].z;
	}
}

/// <summary>
/// �K�E�X�ڂ����̏d�݂̌v�Z
/// </summary>
/// <param name="_pos"> ���W </param>
/// <param name="_rho"> �΍� </param>
/// <returns></returns>
const float& RenderBloom::GaussianDistribution(const glm::vec2& _pos, float _rho) const
{
	return exp(-(_pos.x * _pos.x + _pos.y * _pos.y) / (2.0f * _rho * _rho));
}
