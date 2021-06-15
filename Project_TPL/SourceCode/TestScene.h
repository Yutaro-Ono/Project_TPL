//----------------------------------------------------------------------------------+
// @file        SceneBase.h
// @brief       テストシーン
// @note        SceneBaseクラスを継承
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/24   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "SceneBase.h"
#include <vector>

class TestScene : public SceneBase
{
public:

	TestScene();
	~TestScene();

	void Initialize() override;

	SceneBase* Update(float _deltaTime) override;

private:
	


};