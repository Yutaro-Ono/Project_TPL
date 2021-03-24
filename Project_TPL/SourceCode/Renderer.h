//----------------------------------------------------------------------------------+
// @file        Renderer.h
// @brief       描画処理
//              メッシュ・テクスチャ・シェーダー情報の保持
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/22   新規作成
//----------------------------------------------------------------------------------+
#pragma once
// インクルードファイル
// ※gl3w→glfwの順番に記述
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

class Renderer
{
public:

	Renderer();
	~Renderer();

	bool Initialize();
	void Delete();

	void Draw();


	GLFWwindow* GetMainWindow() { return m_window; }    // ウィンドウコンテキストのゲッター

private:

	static void FrameBuffer_Size_Callback(GLFWwindow* in_window, int in_windth, int in_height);


	GLFWwindow* m_window;     // メインウィンドウ





};