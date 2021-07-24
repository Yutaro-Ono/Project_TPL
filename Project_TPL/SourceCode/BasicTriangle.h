//----------------------------------------------------------------------------------+
// @file        BasicTriangle.h
// @brief       �ȒP�ȎO�p�`�I�u�W�F�N�g�̍쐬�E�`��I�u�W�F�N�g�Ƃ��ēo�^
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/16   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include <string>
#include "Math.h"

class BasicTriangle
{

public:

	BasicTriangle();
	~BasicTriangle();

	void Draw(class GLSLprogram* _shader);

	unsigned int GetTriangleVAO() { return m_vao; }     // �O�p�`�p���_�z��I�u�W�F�N�g�̃Q�b�^�[

	void SetTriangleColor(const Vector3& _color) { m_color = _color; }


private:

	// ���_�o�b�t�@�E�z��p�I�u�W�F�N�g
	unsigned int m_vbo;
	unsigned int m_vao;

	class Texture* m_diffuseMap;

	// ���[���h�ϊ��s��
	Matrix4 m_worldTrans;
	// �g�嗦�A��]�l
	Vector3 m_scale;
	float m_radian;

	// �J���[���
	Vector3 m_color;

	static const std::string TEXTURE_KEY;

};