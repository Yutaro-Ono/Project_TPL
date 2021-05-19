//----------------------------------------------------------------------------------+
// @file        TexturePool.cpp
// @brief       テクスチャプール (テクスチャクラスを管理する)
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/7   新規作成
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
/// プールから指定パスのテクスチャを生成
/// </summary>
/// <param name="_texturePath"> テクスチャパス </param>
/// <returns> テクスチャクラスのポインタ </returns>
Texture* TexturePool::GetTexture(const std::string& _texturePath)
{
	Texture* texture = nullptr;
	auto iter = m_keyObjPool.find(_texturePath);

	if (iter != m_keyObjPool.end())
	{
		// すでに同じテクスチャが生成されていた場合
		return iter->second;
	}
	else
	{
		// テクスチャの生成
		texture = new Texture();

		if (texture->LoadTexture(_texturePath))
		{
			// 成功
			m_keyObjPool.emplace(_texturePath, texture);
		}
		else
		{
			// 失敗
			delete texture;
			texture = nullptr;
		}

	}

	return texture;
}

