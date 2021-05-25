//----------------------------------------------------------------------------------+
// @file        TestSphere.h
// @brief       テスト球体クラス
// @note        アクター
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/19   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "Actor.h"

class TestSphere : public Actor
{

public:

	TestSphere();
	~TestSphere();

	void UpdateActor(float _deltaTime) override;

private:

	class TestSphereDebug* m_debugObj;

};

