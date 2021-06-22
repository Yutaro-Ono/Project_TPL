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
#include "TexturePool.h"
#include "MeshPool.h"
#include "ActorPool.h"

#include <iostream>



/// <summary>
/// コンストラクタ
/// </summary>
GameMain::GameMain()
	:m_texturePool(nullptr)
	,m_meshPool(nullptr)
	,m_actorPool(nullptr)
	,m_renderer(nullptr)
	,m_debugger(nullptr)
	,m_scene(nullptr)
	,m_deltaTime(1.0f)
	,m_currentFrame(glfwGetTime() / 1000.0f)
	,m_lastFrame(glfwGetTime() / 1000.0f)
	,m_bulletTime(1.0f)
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
	if (!GAME_CONFIG.Load("Project_TPL.ini"))
	{
		std::cout << "Error::GameSettings::Load" << std::endl;
		return false;
	}

	// 各種オブジェクトプールの生成
	m_texturePool = new TexturePool();
	m_meshPool = new MeshPool();
	m_actorPool = new ActorPool();

	// レンダラークラスの作成
	m_renderer = new Renderer();
	if (!m_renderer->Initialize(GAME_CONFIG.m_displayWidth, GAME_CONFIG.GetInstance().m_displayHeight, GAME_CONFIG.m_displayFullScreen))
	{
		std::cout << "Error::Renderer::Initialize" << std::endl;
		return false;
	}

#ifdef _DEBUG

	// デバッガ―クラスの作成
	m_debugger = new Debugger();
	if (!m_debugger->Initialize())
	{
		std::cout << "Error::Debugger::Initialize" << std::endl;
		return false;
	}
	// レンダラー用デバッグオブジェクトの生成
	m_renderer->CreateRendererDebugObject();

#endif

	return true;
}

/// <summary>
/// 解放処理
/// </summary>
void GameMain::Delete()
{
	delete m_renderer;
	delete m_texturePool;
	delete m_meshPool;
	delete m_actorPool;

#ifdef _DEBUG

	delete m_debugger;

#endif

}

/// <summary>
/// 入力プロセスの更新処理
/// </summary>
void GameMain::ProcessInput()
{
	// ESCAPEが押されたら
    // ウィンドウ終了フラグをONする
	if (glfwGetKey(m_renderer->GetMainWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_renderer->GetMainWindow(), true);
	}

#ifdef _DEBUG

	// デバッグ画面
	if (glfwGetKey(m_debugger->GetDebugWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_debugger->GetDebugWindow(), true);
	}

#endif
}

/// <summary>
/// 全処理のループ関数
/// </summary>
/// <returns> ループ中で問題が発生した場合falseを返す </returns>
bool GameMain::RunLoop()
{
	//------------------------------------------------+
	// メインループ
	//------------------------------------------------+
	// 入力処理
	ProcessInput();

	// デルタタイム更新
	UpdateDeltaTime();

	// シーンの更新
	SceneBase* currentScene = m_scene->Update(m_deltaTime);
	// 現在シーンが更新されていたら
	if (currentScene != m_scene)
	{
		// 前のシーンを削除
		delete m_scene;
		// メインシーンを更新されたシーンに切り替え・初期化
		m_scene = currentScene;
		m_scene->Initialize();
	}

	// アクターの更新
	m_actorPool->UpdateObjects(m_deltaTime);

#ifdef _DEBUG

	//------------------------------------------------+
	// imgui更新・描画処理 (Debugビルドでのみ行う)
	//------------------------------------------------+
	m_debugger->UpdateImGui(m_deltaTime);

#endif

	//------------------------------------------------+
	// 描画処理
	//------------------------------------------------+
	m_renderer->Draw();

#ifdef _DEBUG

	//------------------------------------------------+
	// imgui描画処理 (Debugビルドでのみ行う)
	//------------------------------------------------+
	m_debugger->RenderImGui();

#endif
	

	//------------------------------------------------+
	// GLイベントの更新
	//------------------------------------------------+
	glfwPollEvents();          // キーボード・マウスのトリガーを確認

    // (GLFWが何らかの操作で閉じるまでループ)
	return !glfwWindowShouldClose(m_renderer->GetMainWindow());
}


/// <summary>
/// フレーム関係の更新処理
/// </summary>
void GameMain::UpdateDeltaTime()
{
	bool updatePermit = false;
	
	// 現在のフレームを取得
	m_currentFrame = glfwGetTime() / 1000.0f;

	if (m_currentFrame >= m_lastFrame + (16.0f / 1000.0f))
	{
		updatePermit = true;
	}

	if (updatePermit)
	{
		// 前フレームから現在フレームまでの経過時間を算出(秒単位)
		m_deltaTime = ((glfwGetTime() / 1000.0f) - m_lastFrame + (16.0f / 1000.0f));
	}

	// フレームを更新
	m_lastFrame = glfwGetTime() / 1000.0f;

	// フレーム時間が経過しすぎている場合は0.05 → 20fps固定
	if (m_deltaTime > 0.05f)
	{
		m_deltaTime = 0.05f;
	}
}

/// <summary>
/// シーンのセット
/// </summary>
/// <param name="_scene"> シーンベースクラス </param>
void GameMain::SetScene(SceneBase* _scene)
{
	if (m_scene == nullptr)
	{
		m_scene = _scene;
	}
	else
	{
		delete m_scene;
		m_scene = _scene;
	}

	m_scene->Initialize();
}