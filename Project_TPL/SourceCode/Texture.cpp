//----------------------------------------------------------------------------------+
// @file        Texture.cpp
// @brief       テクスチャの読み込み・登録・保持・削除
//              
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/23   新規作成
// 2021/ 6/22   ミップマップの生成関数を追加
//----------------------------------------------------------------------------------+
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "GameMain.h"
#include "Renderer.h"
#include "TexturePool.h"

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
    // 画像ファイルのロード
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(_filePath.c_str(), &m_width, &m_height, &m_channels, STBI_rgb_alpha);
    if (data != NULL)
    {
        // テクスチャバッファの生成・ID登録
        glGenTextures(1, &m_textureID);
        glBindTexture(GL_TEXTURE_2D, m_textureID);
        // イメージ読み込み
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        // 解放
        stbi_image_free(data);

        // テクスチャラッピング
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // テクスチャフィルタリング
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // バインド解除
        glBindTexture(GL_TEXTURE_2D, 0);

        // テクスチャプールに追加
        GAME_INSTANCE.GetTexturePool()->AddKeyObject(_filePath, this);

        // ミップマップの作成
        SetMipmap(m_textureID);
    }
    else
    {
        std::cout << "Failed::Load Texture::" << _filePath << std::endl;
        return false;
    }



    return true;
}

/// <summary>
/// キューブマップ用テクスチャのロード
/// ※6面
/// </summary>
/// <param name="_filePath"></param>
/// <returns></returns>
const unsigned int Texture::LoadCubeMapTextures(const std::string& _filePath) const
{
    // キューブマップID
    unsigned int cubeMap;

    // 各面のテクスチャパス
    std::string faces[] =
    {
        _filePath + "_posx.jpg",
        _filePath + "_negx.jpg",
        _filePath + "_posy.jpg",
        _filePath + "_negy.jpg",
        _filePath + "_posz.jpg",
        _filePath + "_negz.jpg"
    };

    int width, height, channels;

    for (unsigned int i = 0; i < 6; ++i)
    {

        // 1面のテクスチャをロード
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);

        if (data != NULL)
        {
            // キューブマップとして登録
            glGenTextures(1, &cubeMap);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            // テクスチャパラメータ設定
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // バインド解除
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

            // 読み込みデータの解放
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed::Load CubeMap Face Texture::" << faces[i] << std::endl;
            return false;
        }

    }

    return cubeMap;
}

/// <summary>
/// HDRテクスチャの読み込み
/// </summary>
/// <param name="_filePath"> テクスチャのパス </param>
/// <returns> 生成したテクスチャID </returns>
const unsigned int Texture::LoadTextureHDR(const std::string& _filePath) const
{
    unsigned int hdrTexture;
    int width, height, nrComponents;

    stbi_set_flip_vertically_on_load(true);
    float* data = stbi_loadf(_filePath.c_str(), &width, &height, &nrComponents, 0);
    
    if (data != NULL)
    {
        glGenTextures(1, &hdrTexture);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

        // テクスチャパラメータ設定
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);
    }
    


    return 0;
}

/// <summary>
/// 指定したテクスチャIDのミップマップを生成
/// </summary>
/// <param name="_textureID"> ミップマップのターゲットとなるテクスチャID </param>
void Texture::SetMipmap(unsigned int _textureID)
{
    glGenerateTextureMipmap(_textureID);

    glBindTexture(GL_TEXTURE_2D, _textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

