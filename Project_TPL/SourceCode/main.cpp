// インクルードファイル
// ※gladの後にglfwを記述
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void FrameBuffer_Size_Callback(GLFWwindow* in_window, int in_windth, int in_height);

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

	//---------------------------------------+
	// GLAD初期化
	//---------------------------------------+
	// OS固有のOpenGL関数ポインタのアドレスをロードする
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed : Initialize GLAD" << std::endl;
		return -1;
	}

	//---------------------------------------+
	// ビューポートの設定
	//---------------------------------------+
	// ビューポート (0x0の座標から1920x1080までを描画範囲として設定)
	glViewport(0, 0, 1920, 1080);
	// ウィンドウサイズ変更が行われた際に、コールバック関数 (今回は画面サイズの最適化関数)を呼び出すことを、GLFWに指示
	glfwSetFramebufferSizeCallback(window, FrameBuffer_Size_Callback);

	// メインループ
	// (GLFWが何らかの操作で閉じるまでループ)
	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);   // 新しいカラーバッファを古いバッファと交換し、画面に表示
		glfwPollEvents();          // キーボード・マウスのトリガーを確認
	}
	
	// GLFWのクリーンアップ
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