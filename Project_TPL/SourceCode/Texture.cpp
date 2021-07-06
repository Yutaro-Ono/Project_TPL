//----------------------------------------------------------------------------------+
// @file        Texture.cpp
// @brief       �e�N�X�`���̓ǂݍ��݁E�o�^�E�ێ��E�폜
//              
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/23   �V�K�쐬
// 2021/ 6/22   �~�b�v�}�b�v�̐����֐���ǉ�
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
/// �e�N�X�`���̃��[�h
/// </summary>
/// <param name="_filePath"> �e�N�X�`���t�@�C���̃p�X </param>
/// <returns> ���[�h�Ɏ��s������false��Ԃ� </returns>
bool Texture::LoadTexture(const std::string& _filePath)
{
    // �摜�t�@�C���̃��[�h
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(_filePath.c_str(), &m_width, &m_height, &m_channels, STBI_rgb_alpha);
    if (data != NULL)
    {
        // �e�N�X�`���o�b�t�@�̐����EID�o�^
        glGenTextures(1, &m_textureID);
        glBindTexture(GL_TEXTURE_2D, m_textureID);
        // �C���[�W�ǂݍ���
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        // ���
        stbi_image_free(data);

        // �e�N�X�`�����b�s���O
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // �e�N�X�`���t�B���^�����O
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // �o�C���h����
        glBindTexture(GL_TEXTURE_2D, 0);

        // �e�N�X�`���v�[���ɒǉ�
        GAME_INSTANCE.GetTexturePool()->AddKeyObject(_filePath, this);

        // �~�b�v�}�b�v�̍쐬
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
/// �L���[�u�}�b�v�p�e�N�X�`���̃��[�h
/// ��6��
/// </summary>
/// <param name="_filePath"></param>
/// <returns></returns>
const unsigned int Texture::LoadCubeMapTextures(const std::string& _filePath) const
{
    // �L���[�u�}�b�vID
    unsigned int cubeMap;

    // �e�ʂ̃e�N�X�`���p�X
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

        // 1�ʂ̃e�N�X�`�������[�h
        stbi_set_flip_vertically_on_load(true);
        float *data = stbi_loadf(faces[i].c_str(), &width, &height, &channels, 0);

        if (data != NULL)
        {
            // �L���[�u�}�b�v�Ƃ��ēo�^
            glGenTextures(1, &cubeMap);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
            // �e�N�X�`���p�����[�^�ݒ�
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // �o�C���h����
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

            // �ǂݍ��݃f�[�^�̉��
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
/// HDR�e�N�X�`���̓ǂݍ���
/// </summary>
/// <param name="_filePath"> �e�N�X�`���̃p�X </param>
/// <returns> ���������e�N�X�`��ID </returns>
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

        // �e�N�X�`���p�����[�^�ݒ�
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
/// �w�肵���e�N�X�`��ID�̃~�b�v�}�b�v�𐶐�
/// </summary>
/// <param name="_textureID"> �~�b�v�}�b�v�̃^�[�Q�b�g�ƂȂ�e�N�X�`��ID </param>
void Texture::SetMipmap(unsigned int _textureID)
{
    glGenerateTextureMipmap(_textureID);

    glBindTexture(GL_TEXTURE_2D, _textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

