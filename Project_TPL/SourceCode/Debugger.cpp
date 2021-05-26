//----------------------------------------------------------------------------------+
// @file        Debugger.cpp
// @brief       デバッグ用UI(imguiを使用)の更新
//              各種デバッグオブジェクトの保持
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//----------------------------------------------------------------------------------+
#include "Debugger.h"
#include "DebugObjectBase.h"
#include "DebugObjectPool.h"
#include <iostream>

/// <summary>
/// コンストラクタ
/// </summary>
Debugger::Debugger()
	:m_debugWindow(NULL)
	,m_windowH(640)
	,m_windowW(480)
	,m_debugObjectPool(nullptr)
{
	m_debugObjectPool = new DebugObjectPool();
}

/// <summary>
/// デストラクタ
/// </summary>
Debugger::~Debugger()
{
	Delete();
}

/// <summary>
/// 初期化処理
/// </summary>
/// <returns> 初期化失敗時にfalseを返す </returns>
bool Debugger::Initialize()
{

	//--------------------------------------+
    // デバッグウィンドウ定義
    //--------------------------------------+
    // ウィンドウ作成 (1920x1080)
	m_debugWindow = glfwCreateWindow(m_windowH, m_windowW, "Debug_TPL", NULL, NULL);
	// ウィンドウ作成失敗時
	if (m_debugWindow == NULL)
	{
		std::cout << "Failed : Create Debugger Window" << std::endl;
		return false;
	}

	//----------------------------------------+
    // imguiセットアップ
    //----------------------------------------+
    // imguiコンテキスト作成
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// imguiのカラースタイルを設定
	ImGui::StyleColorsDark();         // ダークスタイル
	//ImGui::StyleColorsClassic();    // クラシックスタイル
	// 日本語フォントに設定
	io.Fonts->AddFontFromFileTTF("Data/Fonts/Debug/mplus-1c-medium.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

	// GLとGLSLにバインド
	// ver420を使用時でも、410 core としてセット
	const char* glsl_version = "#version 410 core";
	ImGui_ImplGlfw_InitForOpenGL(m_debugWindow, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

    return true;
}

/// <summary>
/// 解放処理
/// </summary>
void Debugger::Delete()
{
	// デバッグオブジェクトのプールを解放
	delete m_debugObjectPool;

	// imguiのクリーンアップ
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	// windowの破棄
	glfwDestroyWindow(m_debugWindow);
}

/// <summary>
/// ImGuiの更新処理
/// デバッグクラス群の更新を行う予定
/// </summary>
void Debugger::UpdateImGui(float _deltaTime)
{
	// アクティブなウィンドウとしてデバッグ画面を設定
	glfwMakeContextCurrent(m_debugWindow);

	// imguiの新規フレームを開始する
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// ウィンドウタイトル
	// ※日本語フォントはu8リテラルで正しく表示できる
	ImGui::Begin("Debug Screen");

	// テキスト表示
	//ImGui::Text("index");

	// テキスト表示 (自動的に改行される)
	//ImGui::Text("index");

	// 改行しないという明示。次のテキストが同じ行に表示される
	//ImGui::SameLine();

	// テキスト
	//ImGui::Text(u8"です。");

	// 区切り線
	//ImGui::Separator();

	// チェックボックス
	//static bool check;
	//ImGui::Checkbox(u8"チェック", &check);

	// 区切り線
	//ImGui::Separator();

	// カラーピッカー
	//static float color;
	//ImGui::ColorPicker4(u8"カラー", &color);

	//フレームレートを表示
	ImGui::Text("Application average %.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Text("FrameRate >> %.1f FPS", ImGui::GetIO().Framerate);

	// デバッグオブジェクトプールの更新処理
	m_debugObjectPool->UpdateObjects(_deltaTime);


	ImGui::End();

	// 描画処理
	RenderImGui();

}

/// <summary>
/// 描画処理
/// デバッグウィンドウに対して描画を行う
/// </summary>
void Debugger::RenderImGui()
{
	// ImGuiレンダリング開始
	ImGui::Render();

	// カラーバッファをクリア
	glClear(GL_COLOR_BUFFER_BIT);

	// フレームバッファサイズの取得
	glfwGetFramebufferSize(m_debugWindow, &m_windowW, &m_windowH);
	// ビューポートの設定
	glViewport(0, 0, m_windowW, m_windowH);

	// フレームバッファにImGuiの描画結果を書き出す
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	// 新しいカラーバッファを古いバッファと交換し、画面に表示
	glfwSwapBuffers(m_debugWindow);
}


void Debugger::AddDebugObject(DebugObjectBase* _debugObj)
{
	m_debugObjectPool->AddObject(_debugObj);
}


void Debugger::DeleteDebugObject(DebugObjectBase* _debugObj)
{
	m_debugObjectPool->DeleteObject(_debugObj);
}


