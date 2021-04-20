//----------------------------------------------------------------------------------+
// @file        VertexArray.h
// @brief       ���_�z���VAO/VBO�ɓo�^����
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/16   �V�K�쐬
//----------------------------------------------------------------------------------+
#include "VertexArray.h"


VertexArray::VertexArray()
{
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="_verts"> ���_�z�� </param>
/// <param name="_vertsNum"> ���_�� </param>
/// <param name="_layout"> ���_���C�A�E�g�̎�� </param>
/// <param name="_inDices"> �C���f�b�N�X�o�b�t�@ </param>
/// <param name="_numInDices"> �C���f�b�N�X�� </param>
VertexArray::VertexArray(const void* _verts, unsigned int _vertsNum, Vertex::LAYOUT_TYPE _layout, const unsigned int* _inDices, unsigned int _numInDices)
	:m_vertsNum(_vertsNum)
	,m_numInDices(_numInDices)
{
	// ���_�z��I�u�W�F�N�g�̓o�^�E�o�C���h
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// ���_�f�[�^�T�C�Y�̌v�Z (�v�Z���ɂ��Ă�Layout�̒�`�̕����ɖ��L)
	unsigned vertexSize = 8 * sizeof(float);    // POS_NORMAL_

	if (_layout == Vertex::LAYOUT_TYPE::POS_NORMAL_UV_TAN)
	{
		vertexSize = 11 * sizeof(float);
	}
	if (_layout == Vertex::LAYOUT_TYPE::POS_NORMAL_SKIN_UV)
	{
		vertexSize = 8 * sizeof(float) + 8 * sizeof(char);
	}
	if (_layout == Vertex::LAYOUT_TYPE::POS_NORMAL_SKIN_UV_TAN)
	{
		vertexSize = 11 * sizeof(float) + 8 * sizeof(char);
	}

}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="_verts"> ���_�z�� </param>
/// <param name="_vertsNum"> ���_�� </param>
/// <param name="_inDices"> �C���f�b�N�X�o�b�t�@ </param>
/// <param name="_numInDices"> �C���f�b�N�X�� </param>
VertexArray::VertexArray(const float* _verts, unsigned int _vertsNum, const unsigned int* _inDices, unsigned int _numInDices)
{
}


VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_indexBuffer);
	glDeleteVertexArrays(1, &m_vao);

}
