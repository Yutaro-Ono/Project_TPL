//----------------------------------------------------------------------------------+
// @file        Texture.h
// @brief       テクスチャの読み込み・登録・保持・削除
//              
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/23   新規作成
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