#include "Renderer.h"
#include <iostream>

/// <summary>
/// コンストラクタ
/// </summary>
Renderer::Renderer()
	:m_window(NULL)
{
}

/// <summary>
/// デストラクタ
/// </summary>
Renderer::~Renderer()
{
	Delete();
}

/// <summary>
/// 初期化処理
/// </summary>
/// <returns> 初期化失敗時にfalseを返す </returns>
bool Renderer::Initialize()
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
	m_window = glfwCreateWindow(1920, 1080, "Project_TPL", NULL, NULL);
	// ウィンドウ作成失敗時
	if (m_window == NULL)
	{
		std::cout << "Failed : Create Renderer Window" << std::endl;
		return false;
	}
	// 作成したウィンドウを現在のスレッドのメインコンテキストとして設定
	glfwMakeContextCurrent(m_window);
	
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
	glfwSetFramebufferSizeCallback(m_window, FrameBuffer_Size_Callback);

	return true;
}

/// <summary>
/// 解放処理
/// </summary>
void Renderer::Delete()
{
	// windowの破棄・GLFWのクリーンアップ
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

/// <summary>
/// 描画関数
/// </summary>
void Renderer::Draw()
{
	// 描画するウィンドウをセット
	glfwMakeContextCurrent(m_window);

	// ビューポートの更新
	glViewport(0, 0, 1920, 1080);

	//------------------------------------------------+
    // 描画処理
    //------------------------------------------------+
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);      // 指定した色値で画面をクリア
	glClear(GL_COLOR_BUFFER_BIT);              // 画面のカラーバッファをクリア
	//glClear(GL_DEPTH_BUFFER_BIT);
	//glClear(GL_STENCIL_BUFFER_BIT);

	// 新しいカラーバッファを古いバッファと交換し、画面に表示
	glfwSwapBuffers(m_window);
}

/// <summary>
/// 画面のリサイズを行う関数 (コールバック関数としてメインループで設定)
/// (画面サイズの変更が行われた際にGLFWにより自動的に呼び出される)
/// </summary>
/// <param name="in_window"> ウィンドウオブジェクトのポインタ </param>
/// <param name="in_windth"> 画面の横幅 </param>
/// <param name="in_height"> 画面の縦幅 </param>
void Renderer::FrameBuffer_Size_Callback(GLFWwindow* in_window, int in_windth, int in_height)
{
	// ビューポートのリサイズ
	glViewport(0, 0, in_windth, in_height);
}
