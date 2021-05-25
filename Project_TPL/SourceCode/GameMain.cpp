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
#include "TestSphere.h"

#include <iostream>

/// <summary>
/// コンストラクタ
/// </summary>
GameMain::GameMain()
	:m_settings(nullptr)
	,m_texturePool(nullptr)
	,m_meshPool(nullptr)
	,m_renderer(nullptr)
	,m_debugger(nullptr)
	,m_scene(nullptr)
	,m_deltaTime(1.0f)
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
	if (!m_settings->GetInstance().Load("Project_TPL.ini"))
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
	if (!m_renderer->Initialize(m_settings->GetInstance().m_displayWidth, m_settings->GetInstance().m_displayHeight, m_settings->GetInstance().m_displayFullScreen))
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

#endif



	TestSphere* sphere = new TestSphere();
	sphere->SetPosition(glm::vec3(10.0f, 5.0f, 10.0f));
	sphere->SetScale(glm::vec3(0.1f));
	TestSphere* sphere1 = new TestSphere();
	sphere1->SetPosition(glm::vec3(-10.0f, 0.0f, 7.0f));
	sphere1->SetScale(glm::vec3(0.1f));
	TestSphere* sphere2 = new TestSphere();
	sphere2->SetPosition(glm::vec3(4.0f, -5.0f, 1.0f));
	sphere2->SetScale(glm::vec3(0.1f));

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

	// メインループ
	//------------------------------------------------+
	// 入力に対する処理
	//------------------------------------------------+
	ProcessInput();      // 入力による処理

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

	//------------------------------------------------+
	// GLイベントの更新
	//------------------------------------------------+
	glfwPollEvents();          // キーボード・マウスのトリガーを確認

    // (GLFWが何らかの操作で閉じるまでループ)
	return !glfwWindowShouldClose(m_renderer->GetMainWindow());
}