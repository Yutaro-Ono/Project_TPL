//----------------------------------------------------------------------------------+
// @file        TexturePool.h
// @brief       テクスチャプール (テクスチャクラスを管理する)
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/7   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include <string>
#include <unordered_map>
#include "ObjectPool.h"
#include "Texture.h"


class TexturePool : public ObjectPool<Texture>
{

public:

	TexturePool();
	~TexturePool();

	void UpdateObjects(float _deltaTime) override;
	
	class Texture* GetTexture(const std::string& _texturePath);

private:





};