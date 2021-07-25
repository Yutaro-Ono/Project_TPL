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
#include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/imgui_impl_sdl.h"
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

	// バッファ情報の取得
	const unsigned int& GetDebugColorBuffer() const { return m_debugCB; }
	const unsigned int& GetDebugFrameBuffer() const { return m_debugFB; }
	// バッファ縦横サイズ取得
	const int& GetDebugBufferWidth() const { return m_debugWidth; }
	const int& GetDebugBufferHeight() const { return m_debugHeight; }

	SDL_Window* GetDebugWindow() { return m_debugWindow; }


private:

	void CreateDebugBuffers();

	
	SDL_Window* m_debugWindow;            // デバッグ用ウィンドウ
	SDL_GLContext m_debugContext;         // デバッグ用コンテキスト
	SDL_Renderer* m_debugRenderer;        // デバッグ用SDLレンダラー    

	class GLSLprogram* m_debugShader;     // デバッグ用シェーダー
	unsigned int m_debugCB;               // デバッグ用カラーバッファ
	unsigned int m_debugRB;               // デバッグ用レンダーバッファ
	unsigned int m_debugFB;               // デバッグ用フレームバッファ
	int m_debugWidth, m_debugHeight;

	int m_windowH;                        // デバッグ用ウィンドウ縦サイズ
	int m_windowW;                        // デバッグ用ウィンドウ横サイズ

	class DebugObjectPool* m_debugObjects;
	class DebugObjectPool* m_actorDebugObjects;

};