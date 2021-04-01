//----------------------------------------------------------------------------------+
// @file        SceneBase.h
// @brief       シーンの基底クラス.
// @note        抽象クラス
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/24   新規作成
//----------------------------------------------------------------------------------+
#pragma once

class SceneBase
{
public:


	SceneBase() {};
	virtual ~SceneBase() {};

	virtual SceneBase* Update(float _deltaTime) = 0;

	virtual void Initialize() = 0;

};