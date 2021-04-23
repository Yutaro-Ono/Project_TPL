//----------------------------------------------------------------------------------+
// @file        Actor.cpp
// @brief       アクタークラス (座標を持ち、描画されるオブジェクト全般の基底クラス)
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/23   新規作成
//----------------------------------------------------------------------------------+
#include "Actor.h"

Actor::Actor()
{
}

Actor::~Actor()
{
}

void Actor::Update(float _deltaTime)
{
}

void Actor::UpdateComponents(float _deltaTime)
{
}

void Actor::UpdateActor(float _deltaTime)
{
}

void Actor::CalcWorldTransform()
{
}
