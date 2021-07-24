//----------------------------------------------------------------------------------+
// @file        ActorDebugObject.h
// @brief       アクター用デバッグオブジェクトクラス
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/25   新規作成
//----------------------------------------------------------------------------------+
#pragma once
// インクルードファイル
// ※gl3w→glfwの順番に記述
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "DebugObjectBase.h"

class ActorDebugObject : public DebugObjectBase
{

public:


	ActorDebugObject(class Actor* _owner);
	~ActorDebugObject();

	void Update(float _deltaTime) override;
	void Render() override;

private:


	class Actor* m_owner;

	std::string m_actorLabel;      // imGui上で識別するために用いるアクターラベル
};