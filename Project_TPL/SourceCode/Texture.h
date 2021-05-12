//----------------------------------------------------------------------------------+
// @file        Texture.h
// @brief       �e�N�X�`���̓ǂݍ��݁E�o�^�E�ێ��E�폜
//              
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/23   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include <GL/gl3w.h>
#include <string>
#include <iostream>


class Texture
{

public:

	Texture();
	~Texture();

	bool LoadTexture(const std::string& _filePath);

	unsigned int GetTextureID() { return m_textureID; }

	


private:

	//void WrappingTexture()
	//void FilteringTexture()

	unsigned int m_textureID;

	int m_width, m_height;
	int m_channels;

};