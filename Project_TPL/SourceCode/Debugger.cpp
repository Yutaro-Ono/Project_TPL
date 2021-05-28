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
	,m_windowH(1280)
	,m_windowW(720)
	,m_debugObjects(nullptr)
	,m_actorDebugObjects(nullptr)
{
	m_debugObjects = new DebugObjectPool();
	m_actorDebugObjects = new DebugObjectPool();
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
    // ウィンドウ作成
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
	delete m_debugObjects;
	delete m_actorDebugObjects;

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

	//-------------------------------------------------------------------------------+
	// デバッグウィンドウ 1 (システム関連)
	//-------------------------------------------------------------------------------+
	// タイトルバーのカラー設定(選択時)
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.7f, 0.0f, 0.7f, 1.0f));
	// タイトルバーのカラー設定(非選択時)
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.4f, 0.0f, 0.4f, 1.0f));
	// ウィンドウのカラー設定
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.3f, 0.0f, 0.3f, 0.3f));
	// ウィンドウの座標設定 (最初の一回のみ)
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	// ウィンドウのサイズ設定
	ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_Once);
	// ウィンドウタイトル
	// ※日本語フォントはu8リテラルで正しく表示できる
	ImGui::Begin(u8"デバッグ画面 (システム)");
	//フレームレートを表示
	ImGui::Text("Application average %.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Text("FrameRate >> %.1f FPS", ImGui::GetIO().Framerate);
	// デバッグオブジェクト更新処理
	m_debugObjects->UpdateObjects(_deltaTime);
	// システムウィンドウ終了
	ImGui::End();
	// ウィンドウのカラー設定を消去
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	//-------------------------------------------------------------------------------+
	// デバッグウィンドウ 2 (アクター関連)
	//-------------------------------------------------------------------------------+
	// タイトルバーのカラー設定(選択時)
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.2f, 0.7f, 1.0f));
	// タイトルバーのカラー設定(非選択時)
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.1f, 0.4f, 1.0f));
	// ウィンドウのカラー設定
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.08f, 0.2f, 0.3f));
	// ウィンドウの座標設定 (最初の一回のみ)
	ImGui::SetNextWindowPos(ImVec2(20, 300), ImGuiCond_Once);
	// ウィンドウのサイズ設定
	ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_Once);
	// ウィンドウタイトル
	ImGui::Begin(u8"デバッグ画面 (アクター)");
	// デバッグオブジェクト更新処理
	m_actorDebugObjects->UpdateObjects(_deltaTime);
	// アクターウィンドウ終了
	ImGui::End();
	// ウィンドウのカラー設定を消去
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	//-------------------------------------------------------------------------------+
	// デバッグウィンドウ 3 (ゲーム設定)
	//-------------------------------------------------------------------------------+
	// タイトルバーのカラー設定(選択時)
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	// タイトルバーのカラー設定(非選択時)
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.4f, 0.1f, 1.0f));
	// ウィンドウのカラー設定
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.2f, 0.08f, 0.3f));
	// ウィンドウの座標設定 (最初の一回のみ)
	ImGui::SetNextWindowPos(ImVec2(620, 20), ImGuiCond_Once);
	// ウィンドウのサイズ設定
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_Once);
	// メニュータブを備えたウィンドウにする
	ImGui::Begin(u8"デバッグ画面 (環境設定)", nullptr, ImGuiWindowFlags_MenuBar);
	// メニュー階層処理
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{

			}
			if (ImGui::MenuItem("Load"))
			{

			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	// 更新処理

	// アクターウィンドウ終了
	ImGui::End();
	// ウィンドウのカラー設定を消去
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();


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


void Debugger::AddDebugObject(DebugObjectBase* _debugObj, OBJECT_TAG _tag)
{

	if (_tag == OBJECT_TAG::SYSTEM)
	{
		m_debugObjects->AddObject(_debugObj);
	}
	else
	{
		m_actorDebugObjects->AddObject(_debugObj);
	}

	
}


void Debugger::DeleteDebugObject(DebugObjectBase* _debugObj, OBJECT_TAG _tag)
{
	if (_tag == OBJECT_TAG::SYSTEM)
	{
		m_debugObjects->DeleteObject(_debugObj);
	}
	else
	{
		m_actorDebugObjects->DeleteObject(_debugObj);
	}
}


