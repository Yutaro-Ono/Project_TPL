//----------------------------------------------------------------------------------+
// @file        Renderer.cpp
// @brief       描画処理
//              メッシュ・テクスチャ・シェーダー情報の保持
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/22   新規作成
//----------------------------------------------------------------------------------+
#include "Renderer.h"
#include <iostream>
#include "BasicTriangle.h"
#include "ShaderManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Renderer::Renderer()
	:m_window(NULL)
	,m_shaderManager(nullptr)
	,m_triangle(nullptr)
	,m_uboMatrices(0)
	,m_uboCamera(0)
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
/// <param name="_width"> スクリーンの横幅 </param>
/// <param name="_height"> スクリーンの縦幅 </param>
/// <param name="_fullScreen"> フルスクリーンにするか </param>
/// <returns> 初期化に失敗した場合falseを返す </returns>
bool Renderer::Initialize(int _width, int _height, bool _fullScreen)
{

	//--------------------------------------+
    // GLFW初期化
    //--------------------------------------+
	if (!glfwInit())
	{
		std::cout << "Error::GLFW Initialize" << std::endl;
		return false;
	}
	// GLFW構成オプションの設定
	// (0：選択するオプション(列挙型), 1：オプションに設定する値)
	// ver4.2を使用。※バージョンがインストールされていない場合、動作しない
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                   // メジャーバージョン
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);                   // マイナーバージョン
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);   // コアプロファイルを使用

	//--------------------------------------+
	// ウィンドウオブジェクト定義
	//--------------------------------------+
	// フルスクリーン化
	if (_fullScreen)
	{
		m_window = glfwCreateWindow(_width, _height, "Project_TPL", glfwGetPrimaryMonitor(), NULL);
	}
	else
	{
		m_window = glfwCreateWindow(_width, _height, "Project_TPL", NULL, NULL);
	}
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
	GLenum error = gl3wInit();
	if (GL3W_OK != error)
	{
		std::cout << "Error::GL3W Initialize" << std::endl;
		return false;
	}

	//---------------------------------------+
	// ビューポートの設定
	//---------------------------------------+
	// ビューポート (0x0の座標から1920x1080までを描画範囲として設定)
	glViewport(0, 0, _width, _height);
	// ウィンドウサイズ変更が行われた際に、コールバック関数 (今回は画面サイズの最適化関数)を呼び出すことを、GLFWに指示
	glfwSetFramebufferSizeCallback(m_window, FrameBuffer_Size_Callback);


	//---------------------------------------+
	// 行列の初期化
	//---------------------------------------+
	m_viewMat = glm::lookAt(glm::vec3(0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(00, 1.0, 0.0));
	m_projMat = glm::perspective(glm::radians(75.0f), (float)_width / (float)_height, 0.1f, 10000.0f);

	//---------------------------------------+
	// uniformバッファ生成
	//---------------------------------------+
	// ビュー行列・プロジェクション行列UBO
	glGenBuffers(1, &m_uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_uboMatrices, 0, 2 * sizeof(glm::mat4));

	// カメラ情報FBO
	glGenBuffers(1, &m_uboCamera);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboCamera);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3::x) + sizeof(glm::vec3::y) + sizeof(glm::vec3::z), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, m_uboCamera, 0, sizeof(glm::vec3::x) + sizeof(glm::vec3::y) + sizeof(glm::vec3::z));

	// シェーダーマネージャー
	m_shaderManager = new ShaderManager();
	if (!m_shaderManager->CreateShaders())
	{
		std::cout << "Error::ShaderManager CreateShaders()" << std::endl;
		return false;
	}

	// デバッグ用三角形
	m_triangle = new BasicTriangle();

	return true;
}

/// <summary>
/// 解放処理
/// </summary>
void Renderer::Delete()
{
	delete m_triangle;
	delete m_shaderManager;

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);              // 画面のカラー・深度・ステンシルバッファをクリア

	// (デバッグ用)三角形の描画
	//m_shaderManager->EnableShaderProgram(GLSLshader::SIMPLE_POS_COLOR);
	m_shaderManager->EnableShaderProgram(GLSLshader::SIMPLE_POS_TEXTURE);
	m_triangle->Draw(m_shaderManager->GetShader(GLSLshader::SIMPLE_POS_TEXTURE));

	// 新しいカラーバッファを古いバッファと交換し、画面に表示
	glfwSwapBuffers(m_window);
}

/// <summary>
/// uniformバッファへの変数セット関数
/// </summary>
void Renderer::SetUniformBuffer()
{
	// 行列UBO
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &m_viewMat);
	glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), sizeof(glm::mat4), &m_projMat);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// カメラUBO
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboCamera);

}

/// <summary>
/// 画面のリサイズを行う関数 (コールバック関数としてメインループで設定)
/// (画面サイズの変更が行われた際にGLFWにより自動的に呼び出される)
/// </summary>
/// <param name="in_window"> ウィンドウオブジェクトのポインタ </param>
/// <param name="in_windth"> 画面の横幅 </param>
/// <param name="in_height"> 画面の縦幅 </param>
void Renderer::FrameBuffer_Size_Callback(GLFWwindow* _window, int _width, int _height)
{
	// ビューポートのリサイズ
	glViewport(0, 0, _width, _height);
}
