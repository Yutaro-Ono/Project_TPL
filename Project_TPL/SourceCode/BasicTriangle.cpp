//----------------------------------------------------------------------------------+
// @file        BasicTriangle.h
// @brief       �ȒP�ȎO�p�`�I�u�W�F�N�g�̍쐬�E�`��I�u�W�F�N�g�Ƃ��ēo�^
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/16   �V�K�쐬
//----------------------------------------------------------------------------------+
#include "BasicTriangle.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "VertexArray.h"
#include "GLSLprogram.h"

BasicTriangle::BasicTriangle()
	:m_vao(0)
	,m_vbo(0)
	,m_shader(nullptr)
{

	// ���[���h�ϊ��s��
	m_worldMat = glm::mat4(1.0);

	// �V�F�[�_�[�N���X�̐���
	m_shader = new GLSLprogram("Shaders/BasicShader.vert", "Shaders/BasicShader.frag");

	// �ȒP�ȎO�p�`�̒��_�z��
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	// ���_�z��E�o�b�t�@��OpenGL�ɓo�^
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	// VAO�Ƀo�C���h
	glBindVertexArray(m_vao);

	// �o�b�t�@�ɒ��_���𑗂�
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// ���_�z��̏����A�g���r���[�g�ɃZ�b�g
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// �e�o�C���h��؂�
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

BasicTriangle::~BasicTriangle()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
	delete m_shader;
}

/// <summary>
/// �`�揈��
/// </summary>
void BasicTriangle::Draw()
{
	m_shader->UseProgram();

	// ���j�t�H�[��
	m_shader->SetUniform("u_worldMat", m_worldMat);
	

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
