//----------------------------------------------------------------------------------+
// @file        Texture.cpp
// @brief       テクスチャの読み込み・登録・保持・削除
//              
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/23   新規作成
//----------------------------------------------------------------------------------+
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture()
    :m_textureID(0)
    ,m_width(0)
    ,m_height(0)
    ,m_channels(0)
{
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_textureID);
}


/// <summary>
/// テクスチャのロード
/// </summary>
/// <param name="_filePath"> テクスチャファイルのパス </param>
/// <returns> ロードに失敗したらfalseを返す </returns>
bool Texture::LoadTexture(const std::string& _filePath)
{
    // テクスチャバッファの生成・ID登録
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    // テクスチャラッピング
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // テクスチャフィルタリング
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 画像ファイルのロード
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(_filePath.c_str(), &m_width, &m_height, &m_channels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed::Load Texture" << std::endl;
        return false;
    }

    // 解放
    stbi_image_free(data);

    return true;
}
