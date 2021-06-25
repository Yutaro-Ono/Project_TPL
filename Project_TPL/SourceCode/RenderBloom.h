//----------------------------------------------------------------------------------+
// @file        RenderBloom.h
// @brief       �u���[���`�揈��
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 6/04   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include <vector>
#include <glm/glm.hpp>

class RenderBloom
{

public:

	RenderBloom();
	~RenderBloom();

	void DownSampling(unsigned int _brightBuffer, class GLSLprogram* _downSampleShader, class VertexArray* _screenVA);
	void GaussBlur(class GLSLprogram* _gaussShader, class VertexArray* _screenVA);
	void DrawBlendBloom(unsigned int _blendBuffer, class GLSLprogram* _bloomShader, class VertexArray* _screenVA);

private:

	void CreateBlurFBO();

	void CalcGaussBlurParam(int _w, int _h, const glm::vec2& _dir, float _deviation, glm::vec3* _offset);
	const  float& GaussianDistribution(const glm::vec2& _pos, float _rho) const;

	// �k���o�b�t�@�ƃe�N�X�`���z��
	std::vector<unsigned int> m_blurFBOs;
	std::vector<unsigned int> m_blurTextures;

	// �g�[���}�b�s���O���̘I�o��
	float m_exposure;

	// �K���}�R���N�V����
	float m_gamma;

};