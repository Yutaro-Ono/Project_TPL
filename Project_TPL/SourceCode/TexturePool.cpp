//----------------------------------------------------------------------------------+
// @file        TexturePool.cpp
// @brief       �e�N�X�`���v�[�� (�e�N�X�`���N���X���Ǘ�����)
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/7   �V�K�쐬
//----------------------------------------------------------------------------------+
#include "TexturePool.h"

TexturePool::TexturePool()
{
}

TexturePool::~TexturePool()
{
	Delete();
}

void TexturePool::UpdateObjects(float _deltaTime)
{
}

/// <summary>
/// �v�[������w��p�X�̃e�N�X�`���𐶐�
/// </summary>
/// <param name="_texturePath"> �e�N�X�`���p�X </param>
/// <returns> �e�N�X�`���N���X�̃|�C���^ </returns>
Texture* TexturePool::GetTexture(const std::string& _texturePath)
{
	Texture* texture = nullptr;
	auto iter = m_keyObjPool.find(_texturePath);

	if (iter != m_keyObjPool.end())
	{
		// ���łɓ����e�N�X�`������������Ă����ꍇ
		return iter->second;
	}
	else
	{
		// �e�N�X�`���̐���
		texture = new Texture();

		if (texture->LoadTexture(_texturePath))
		{
			// ����
			m_keyObjPool.emplace(_texturePath, texture);
		}
		else
		{
			// ���s
			delete texture;
			texture = nullptr;
		}

	}

	return texture;
}

