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
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include "VertexArray.h"
#include "GLSLprogram.h"

CubeMap::CubeMap()
	:m_cubeVerts(nullptr)
{
	m_cubeVerts = new VertexArray();
	m_cubeVerts->CreateCubeMapVertices();
}

CubeMap::~CubeMap()
{
	delete m_cubeVerts;
}

/// <summary>
/// �L���[�u�}�b�v�p�e�N�X�`���̃��[�h
/// ��6��
/// </summary>
/// <param name="_filePath"></param>
/// <returns></returns>
bool CubeMap::LoadCubeMapTextures(const std::string& _filePath)
{
	// �L���[�u�}�b�v�Ƃ��ēo�^
	glGenTextures(1, &m_cubeMapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapID);

	// �e�ʂ̃e�N�X�`���p�X
	std::string faces[] =
	{
		_filePath + "_right.jpg",
		_filePath + "_left.jpg",
		_filePath + "_top.jpg",
		_filePath + "_bottom.jpg",
		_filePath + "_front.jpg",
		_filePath + "_back.jpg"
	};

	int width, height, channels;

	for (unsigned int i = 0; i < 6; ++i)
	{
		
		// 1�ʂ̃e�N�X�`�������[�h
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);

		if (data != NULL)
		{
			// ���������Ƃ��ĕۑ�
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
				         width, height, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		else
		{
			std::cout << "Failed::Load CubeMap Face Texture::" << faces[i] << std::endl;
			return false;
		}

		// ���
		stbi_image_free(data);

	}

	// �e�N�X�`���p�����[�^�ݒ�
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// �o�C���h����
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}

/// <summary>
/// �L���[�u�}�b�v�`�揈��
/// </summary>
/// <param name="_shader"> �L���[�u�}�b�v�V�F�[�_�[ </param>
void CubeMap::Draw(GLSLprogram* _shader)
{

	// �[�x�ݒ�
	glDepthFunc(GL_LEQUAL);

	// �L���[�u�}�b�v�p�e�N�X�`���̃o�C���h
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapID);
	// �L���[�u�}�b�v�̒��_�z����A�N�e�B�u�ɂ���
	m_cubeVerts->SetActive();
	// �`��
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDepthFunc(GL_LESS);
}
