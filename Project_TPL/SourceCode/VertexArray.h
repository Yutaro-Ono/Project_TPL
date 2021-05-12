//----------------------------------------------------------------------------------+
// @file        VertexArray.h
// @brief       ���_�z���VAO/VBO�ɓo�^����
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/16   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

namespace Vertex
{
	//-------------------------------------------------------------------------------------------------------------------+
    // ���_���C�A�E�g�̃f�[�^�T�C�Y
    // POS_NORMAL_UV = 8 * sizeof(float) = 32 bytes
    // | POSITION  | NORMAL    | TEXTURE |
    // | x | y | z | x | y | z | u  | v  | 
    //
	// ���^���W�F���g��Ԃ͒ʏ�̍��W�Ɠ��� <3 * sizeof(float) = 12byte>
	//
    // POS_NORMAL_SKIN_UV
    // POS_NORMAL_SKIN_UV = (8 * sizeof(float)) + (8 * sizeof(char)) = 40 bytes
    // | POSITION  | NORMAL    | BONES   | WEIGHT  |TEXTURE|
    // | x | y | z | x | y | z | char[4] | char[4] | u | v |
    //                                    ��WEIGHT�̊m�ۂ�char�����A���x���K�v�Ȃ��̂�8bit�Œ菬���Ƃ��Ďg�p����
    //-------------------------------------------------------------------------------------------------------------------+
	// ���_���C�A�E�g (�A�g���r���[�g�ɓo�^���钸�_�̃f�[�^�`��)
	enum LAYOUT_TYPE
	{

		POS_NORMAL_UV,                     // ���W�A�@���AUV���W
		POS_NORMAL_UV_TAN,                 // ���W�A�@���AUV���W�A�^���W�F���g���
		POS_NORMAL_SKIN_UV,                // ���W�A�@���AUV���W�A�X�L�����b�V���̃{�[���Əd�ݏ��
		POS_NORMAL_SKIN_UV_TAN             // ���W�A�@���AUV���W�A�X�L�����b�V���̃{�[���Əd�ݏ��A�^���W�F���g���

	};
}


class VertexArray
{

public:

	VertexArray();
	VertexArray(const void* _verts, unsigned int _vertsNum, Vertex::LAYOUT_TYPE _layout,
		                        const unsigned int* _inDices, unsigned int _numInDices);

	VertexArray(const float* _verts, unsigned int _vertsNum,
		        const unsigned int* _inDices, unsigned int _numInDices);

	~VertexArray();


private:


	unsigned int m_vertsNum;                          // ���_��
	unsigned int m_numInDices;                        // �C���f�b�N�X��
	unsigned int m_vao;                               // ���_�z��I�u�W�F�N�g(ID)
	unsigned int m_vbo;                               // ���_�o�b�t�@�I�u�W�F�N�g(ID)
	unsigned int m_indexBuffer;                       // �C���f�b�N�X�o�b�t�@(ID)

};