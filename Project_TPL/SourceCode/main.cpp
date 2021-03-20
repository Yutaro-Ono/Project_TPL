// インクルードファイル
// ※gladの後にglfwを記述
// ※gl3wをglfwより先に記述
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"


// 関数プロトタイプ宣言
void FrameBuffer_Size_Callback(GLFWwindow* in_window, int in_windth, int in_height);
void ProcessInput(GLFWwindow* in_window);
void UpdateImGui();
void RenderImGui(GLFWwindow* in_window);



int main()
{

	//--------------------------------------+
	// GLFW初期化
	//--------------------------------------+
	glfwInit();
	// GLFW構成オプションの設定
	// (0：選択するオプション(列挙型), 1：オプションに設定する値)
	// ver4.2を使用。※バージョンがインストールされていない場合、動作しない
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                   // メジャーバージョン
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);                   // マイナーバージョン
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);   // コアプロファイルを使用


	//--------------------------------------+
	// ウィンドウオブジェクト定義
	//--------------------------------------+
	// ウィンドウ作成 (1920x1080)
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Project_TPL", NULL, NULL);
	// ウィンドウ作成失敗時
	if (window == NULL)
	{
		std::cout << "Failed : Create Window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// 作成したウィンドウを現在のスレッドのメインコンテキストとして設定
	glfwMakeContextCurrent(window);

	//---------------------------------------------------+
	// gl3w初期化 (glfwMakeContextCurrent関数の後に)
	//---------------------------------------------------+
	gl3wInit();
	


	//---------------------------------------+
	// ビューポートの設定
	//---------------------------------------+
	// ビューポート (0x0の座標から1920x1080までを描画範囲として設定)
	glViewport(0, 0, 1920, 1080);
	// ウィンドウサイズ変更が行われた際に、コールバック関数 (今回は画面サイズの最適化関数)を呼び出すことを、GLFWに指示
	glfwSetFramebufferSizeCallback(window, FrameBuffer_Size_Callback);


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
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);


	// メインループ
	// (GLFWが何らかの操作で閉じるまでループ)
	while (!glfwWindowShouldClose(window))
	{

		//------------------------------------------------+
        // 入力に対する処理
        //------------------------------------------------+
		ProcessInput(window);      // 入力による処理


		//------------------------------------------------+
		// 描画処理
		//------------------------------------------------+
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);      // 指定した色値で画面をクリア
		glClear(GL_COLOR_BUFFER_BIT);              // 画面のカラーバッファをクリア
		//glClear(GL_DEPTH_BUFFER_BIT);
		//glClear(GL_STENCIL_BUFFER_BIT);


#ifdef _DEBUG

		//------------------------------------------------+
		// imgui更新・描画処理
		//------------------------------------------------+
		UpdateImGui();
		RenderImGui(window);


#elif _RELEASE


#endif







		//------------------------------------------------+
		// GLイベントの更新とバッファの交換
		//------------------------------------------------+
		glfwPollEvents();          // キーボード・マウスのトリガーを確認
		glfwSwapBuffers(window);   // 新しいカラーバッファを古いバッファと交換し、画面に表示




	}
	
	// imguiのクリーンアップ
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// GLFWのクリーンアップ
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
}

/// <summary>
/// 画面サイズの最適化関数 (コールバック関数としてメインループで設定)
/// (画面サイズの変更が行われた際にGLFWにより自動的に呼び出される)
/// </summary>
/// <param name="in_window"> ウィンドウオブジェクトのポインタ </param>
/// <param name="in_windth"> 画面の横幅 </param>
/// <param name="in_height"> 画面の縦幅 </param>
void FrameBuffer_Size_Callback(GLFWwindow* in_window, int in_windth, int in_height)
{
	glViewport(0, 0, in_windth, in_height);
}

/// <summary>
/// 入力プロセス
/// </summary>
/// <param name="in_window"> ウィンドウオブジェクトのポインタ </param>
void ProcessInput(GLFWwindow* in_window)
{
	// ESCAPEが押されたら
	// ウィンドウ終了フラグをONする
	if (glfwGetKey(in_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(in_window, true);
	}

}

/// <summary>
/// imguiの更新処理
/// </summary>
void UpdateImGui()
{
	// imguiの新規フレームを開始する
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// ウィンドウタイトル
	// ※日本語フォントはu8リテラルで正しく表示できる
	ImGui::Begin(u8"デバッグ画面");

	// 項目
	ImGui::Text("index");



	ImGui::End();
}

/// <summary>
/// imguiの描画処理
/// </summary>
/// <param name="in_window"> 描画するウィンドウ </param>
void RenderImGui(GLFWwindow* in_window)
{
	ImGui::Render();

	int w = 1920;
	int h = 1080;
	// フレームバッファサイズの取得
	glfwGetFramebufferSize(in_window, &w, &h);
	// ビューポートの設定
	glViewport(0, 0, w, h);

	// フレームバッファにImGuiの描画結果を書き出す
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}