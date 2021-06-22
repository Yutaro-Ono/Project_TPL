//----------------------------------------------------------------------------------+
// @file        CubeMap.cpp
// @brief       �L���[�u�}�b�v�̒�`�E���̕ێ�
//              
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 6/22   �V�K�쐬
//----------------------------------------------------------------------------------+
#include "CubeMap.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "VertexArray.h"
#include "GLSLprogram.h"
#include "Texture.h"

CubeMap::CubeMap(const std::string& _filePath)
	:m_cubeVerts(nullptr)
{
	m_cubeVerts = new VertexArray();
	m_cubeVerts->CreateCubeMapVertices();

	Texture* tex = new Texture;
	m_cubeMapID = tex->LoadCubeMapTextures(_filePath);

}

CubeMap::~CubeMap()
{
	delete m_cubeVerts;
}


/// <summary>
/// �L���[�u�}�b�v�`�揈��
/// </summary>
/// <param name="_shader"> �L���[�u�}�b�v�V�F�[�_�[ </param>
void CubeMap::Draw(GLSLprogram* _shader)
{

	// �[�x�ݒ�
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);

	// �L���[�u�}�b�v�p�e�N�X�`���̃o�C���h
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapID);
	// �L���[�u�}�b�v�̒��_�z����A�N�e�B�u�ɂ���
	m_cubeVerts->SetActive();
	// �`��
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}
