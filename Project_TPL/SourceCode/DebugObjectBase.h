//----------------------------------------------------------------------------------+
// @file        DebugObjectBase.h
// @brief       デバッグオブジェクトベースクラス
// @note        imguiによるデバッグ系の基底クラス
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/23   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_opengl3.h"

class DebugObjectBase
{

public:

	DebugObjectBase();
	virtual~DebugObjectBase();

	virtual void Update(float _deltaTime) = 0;

	virtual void Render() = 0;


protected:


};
