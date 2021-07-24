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
#include <GL/glew.h>
#include <string>
#include <iostream>


class Texture
{

public:

	Texture();
	~Texture();

	bool LoadTexture(const std::string& _filePath);
	const unsigned int LoadCubeMapTextures(const std::string& _filePath) const;
	const unsigned int LoadTextureHDR(const std::string& _filePath) const;

	unsigned int GetTextureID() { return m_textureID; }

	void SetMipmap(unsigned int _textureID);


private:

	//void WrappingTexture()
	//void FilteringTexture()

	unsigned int m_textureID;

	int m_width, m_height;
	int m_channels;

};