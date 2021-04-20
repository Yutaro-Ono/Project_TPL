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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "BasicTriangle.h"

class Renderer
{
public:

	Renderer();
	~Renderer();

	bool Initialize(int _width, int _height, bool _fullScreen);
	void Delete();

	void Draw();


	GLFWwindow* GetMainWindow() { return m_window; }    // ウィンドウコンテキストのゲッター

private:

	static void FrameBuffer_Size_Callback(GLFWwindow* _window, int _width, int _height);


	GLFWwindow* m_window;                     // メインウィンドウ

	class BasicTriangle* m_triangle;          // デバッグ用三角形

	// 描画用の行列関連
	glm::mat4 m_viewMat;                      // ビュー行列
	glm::mat4 m_projMat;                      // プロジェクション行列

	// uniformバッファ
	unsigned int m_uboMatrices;               // ビュー・プロジェクション行列用UBO
	unsigned int m_uboCamera;                 // カメラ情報

};