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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class BasicTriangle
{

public:

	BasicTriangle();
	~BasicTriangle();

	void Draw();

	unsigned int GetTriangleVAO() { return m_vao; }     // �O�p�`�p���_�z��I�u�W�F�N�g�̃Q�b�^�[


private:

	// ���_�o�b�t�@�E�z��p�I�u�W�F�N�g
	unsigned int m_vbo;
	unsigned int m_vao;

	// �s��֘A
	glm::mat4 m_worldMat;
	glm::mat4 m_scaleMat;
	glm::mat4 m_rotationMat;

	class GLSLprogram* m_shader;       // �V�F�[�_�[

};