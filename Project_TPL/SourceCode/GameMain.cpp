//----------------------------------------------------------------------------------+
// @file        GameMain.h
// @brief       ゲームループに関わるインスタンスを生成・保持
//              更新処理も行う
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//----------------------------------------------------------------------------------+
#include "GameMain.h"
#include "GameSettings.h"
#include "Renderer.h"
#include "Debugger.h"
#include "SceneBase.h"

#include <iostream>

/// <summary>
/// コンストラクタ
/// </summary>
GameMain::GameMain()
	:m_settings(nullptr)
	,m_renderer(nullptr)
	,m_debugger(nullptr)
	,m_scene(nullptr)
{

}

/// <summary>
/// デストラクタ
/// </summary>
GameMain::~GameMain()
{
	Delete();
}

/// <summary>
/// ライブラリ初期化・各種インスタンスの生成
/// </summary>
/// <returns> 初期化失敗時にfalseを返す </returns>
bool GameMain::Initialize()
{
	// ゲーム設定クラスの読み込み処理
	if (m_settings->GetInstance().Load("Project_TPL.ini"))
	{
		std::cout << "Error::GameSettings::Load" << std::endl;
		return false;
	}

	m_renderer = new Renderer();
	if (!m_renderer->Initialize())
	{
		std::cout << "Error::Renderer::Initialize" << std::endl;
		return false;
	}
	
#ifdef _DEBUG

	m_debugger = new Debugger();
	if (!m_debugger->Initialize())
	{
		std::cout << "Error::Debugger::Initialize" << std::endl;
		return false;
	}

#endif


	return true;
}

/// <summary>
/// 解放処理
/// </summary>
void GameMain::Delete()
{
	delete m_renderer;
	delete m_debugger;
}

/// <summary>
/// 入力プロセスの更新処理
/// </summary>
void GameMain::ProcessInput()
{
	// ESCAPEが押されたら
    // ウィンドウ終了フラグをONする
	if (glfwGetKey(m_renderer->GetMainWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS || 
		glfwGetKey(m_debugger->GetDebugWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_debugger->GetDebugWindow(), true);
		glfwSetWindowShouldClose(m_renderer->GetMainWindow(), true);
		
	}
}

/// <summary>
/// 全処理のループ関数
/// </summary>
/// <returns> ループ中で問題が発生した場合falseを返す </returns>
bool GameMain::RunLoop()
{

	// メインループ
	//------------------------------------------------+
	// 入力に対する処理
	//------------------------------------------------+
	ProcessInput();      // 入力による処理

#ifdef _DEBUG

	//------------------------------------------------+
	// imgui更新・描画処理 (Debugビルドでのみ行う)
	//------------------------------------------------+
	m_debugger->UpdateImGui();

#endif

	//------------------------------------------------+
	// GLイベントの更新
	//------------------------------------------------+
	glfwPollEvents();          // キーボード・マウスのトリガーを確認

    // (GLFWが何らかの操作で閉じるまでループ)
	return !glfwWindowShouldClose(m_renderer->GetMainWindow());
}