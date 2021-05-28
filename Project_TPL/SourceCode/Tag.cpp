//----------------------------------------------------------------------------------+
// @file        Tag.h
// @brief       タグクラス
// @note        ゲーム内アクターを種類分けする
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/28   新規作成
//----------------------------------------------------------------------------------+
#include "Tag.h"

OBJECT_TAG& operator++(OBJECT_TAG& rhs)
{
    if (rhs == OBJECT_TAG::END)
    {
        throw std::out_of_range("for outof range Tag& operator ++ (Tag&)");
    }
    rhs = OBJECT_TAG(static_cast<std::underlying_type<OBJECT_TAG>::type>(rhs) + 1);

    return rhs;
}
