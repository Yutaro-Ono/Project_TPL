//----------------------------------------------------------------------------------+
// @file        Tag.h
// @brief       タグクラス
// @note        ゲーム内要素を種類分けする
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/28   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include <string>
#include <stdexcept>
#include <iterator>

enum class OBJECT_TAG : unsigned char
{
	BEGIN,

	SYSTEM = BEGIN,

	CAMERA,

	LIGHT,

	PLAYER,

	STATIC_OBJECT,

	DEBUG_SPHERE,

	END
};


// インクリメント (全てのタググループのループが可能)
OBJECT_TAG& operator++(OBJECT_TAG& rhs);