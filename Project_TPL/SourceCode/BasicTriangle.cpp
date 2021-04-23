//----------------------------------------------------------------------------------+
// @file        BasicTriangle.h
// @brief       �ȒP�ȎO�p�`�I�u�W�F�N�g�̍쐬�E�`��I�u�W�F�N�g�Ƃ��ēo�^
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/16   �V�K�쐬
// 2021/ 4/20   �J���[���ǉ�
// 2021/ 4/23   �e�N�X�`�����W�ǉ�
//----------------------------------------------------------------------------------+
#include "BasicTriangle.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "VertexArray.h"
#include "GLSLprogram.h"
#include "Texture.h"

BasicTriangle::BasicTriangle()
	:m_vao(0)
	,m_vbo(0)
	,m_diffuseMap(nullptr)
{

	// �X�P�[��(x, y, z)
	m_scale = glm::vec3(1.0);

	// �ʓx
	m_radian = 1.0f;

	// ���[���h�ϊ��s��
	m_worldTrans = glm::mat4(1.0);


	// �J���[
	m_color = glm::vec3(0.0, 1.0, 0.0);

	// ���_�z��̒�` (���W�A�J���[�AUV)
	float vertices[] =
	{
	//  |  x  |  y  |  z  |  u  |  v  |     R     |     G     |     B     
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, m_color.x, m_color.y, m_color.z,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, m_color.x, m_color.y, m_color.z,
		 0.0f,  0.5f, 0.0f, 0.5f, 1.0f, m_color.x, m_color.y, m_color.z
	};


	// ���_�z��E�o�b�t�@��OpenGL�ɓo�^
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	// VAO�Ƀo�C���h
	glBindVertexArray(m_vao);

	// �o�b�t�@�ɒ��_���𑗂�
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);


	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// ���_���W���A�g���r���[�g�ɃZ�b�g
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// �e�N�X�`�����W���A�g���r���[�g�ɃZ�b�g
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// �J���[���̑��M
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	// �e�o�C���h��؂�
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// �e�N�X�`���̐���
	m_diffuseMap = new Texture();
	m_diffuseMap->LoadTexture("Data/Textures/Test/TestTexture.jpg");

}

BasicTriangle::~BasicTriangle()
{
	delete m_diffuseMap;
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

/// <summary>
/// �`�揈��
/// </summary>
void BasicTriangle::Draw(GLSLprogram* _shader)
{

	// �s��v�Z (��]���g�嗦)
	// m_worldTrans = glm::rotate(m_worldTrans, glm::radians(m_radian), glm::vec3(0.0, 0.0, 1.0));
	m_worldTrans = glm::scale(m_worldTrans, m_scale);

	// uniform�ւ̃Z�b�g
	_shader->SetUniform("u_worldTransform", m_worldTrans);

	// �e�N�X�`���̃o�C���h
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_diffuseMap->GetTextureID());

	// �O�p�`�̕`��
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
