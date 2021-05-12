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

