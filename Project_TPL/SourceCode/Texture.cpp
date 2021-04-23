//----------------------------------------------------------------------------------+
// @file        Texture.cpp
// @brief       �e�N�X�`���̓ǂݍ��݁E�o�^�E�ێ��E�폜
//              
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/23   �V�K�쐬
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
/// �e�N�X�`���̃��[�h
/// </summary>
/// <param name="_filePath"> �e�N�X�`���t�@�C���̃p�X </param>
/// <returns> ���[�h�Ɏ��s������false��Ԃ� </returns>
bool Texture::LoadTexture(const std::string& _filePath)
{
    // �e�N�X�`���o�b�t�@�̐����EID�o�^
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    // �e�N�X�`�����b�s���O
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // �e�N�X�`���t�B���^�����O
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // �摜�t�@�C���̃��[�h
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

    // ���
    stbi_image_free(data);

    return true;
}
