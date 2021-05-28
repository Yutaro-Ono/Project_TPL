//----------------------------------------------------------------------------------+
// @file        Debugger.h
// @brief       デバッグ用UI(imguiを使用)の更新
//              各種デバッグオブジェクトの保持
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/24   新規作成
// 2021/ 3/24   デバッグ画面にImGuiが表示されるようにした
//----------------------------------------------------------------------------------+
#pragma once
// インクルードファイル
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include <vector>
#include "Renderer.h"
#include "Tag.h"



class Debugger
{

public:

	Debugger();
	~Debugger();

	bool Initialize();

	void Delete();

	void UpdateImGui(float _deltaTime);

	void RenderImGui();

	void AddDebugObject(class DebugObjectBase* _debugObj, OBJECT_TAG _tag);
	void DeleteDebugObject(class DebugObjectBase* _debugObj, OBJECT_TAG _tag);


	GLFWwindow* GetDebugWindow() { return m_debugWindow; }


private:

	
	GLFWwindow* m_debugWindow;     // デバッグ用ウィンドウ

	int m_windowH;                 // デバッグ用ウィンドウ縦サイズ
	int m_windowW;                 // デバッグ用ウィンドウ横サイズ

	class DebugObjectPool* m_debugObjects;
	class DebugObjectPool* m_actorDebugObjects;

};