//----------------------------------------------------------------------------------+
// @file        Renderer.cpp
// @brief       描画処理
//              メッシュ・テクスチャ・シェーダー情報の保持
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/22   新規作成
// 2021/ 5/26   各種バッファの初期化処理を追加 (Gバッファ・ライト・MSAA)
//----------------------------------------------------------------------------------+
#include "Renderer.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "GameMain.h"
#include "GameSettings.h"
#include "ShaderManager.h"
#include "DrawableObjectManager.h"
#include "RenderBloom.h"
#include "Actor.h"
#include "Debugger.h"
#include "Tag.h"
#include "VertexArray.h"
#include "CubeMap.h"
#include "DirectionalLight.h"
#include "RenderMethodBase.h"
#include "RenderForward.h"
#include "RenderDeferred.h"

/// <summary>
/// コンストラクタ
/// </summary>
Renderer::Renderer()
	:m_window(NULL)
	,m_renderMethodType(RENDER_METHOD::DEFERRED)
	,m_shaderManager(nullptr)
	,m_drawableObject(nullptr)
	,m_bloomRender(nullptr)
	,m_skyBox(nullptr)
	,m_uboMatrices(0)
	,m_uboCamera(0)
	,m_uboTriggers(0)
	,m_uboDirLights(0)
	,m_debugObj(nullptr)
	,m_quadVA(nullptr)
	,m_enableBloom(GAME_CONFIG.GetEnableBloom())
	,m_visualizeNormal(false)
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
	m_viewMat = glm::lookAt(glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0));
	m_projMat = glm::perspective(glm::radians(75.0f), (float)_width / (float)_height, 0.1f, 10000.0f);


	return true;
}

bool Renderer::Load()
{

	//---------------------------------------+
	// 汎用頂点配列
	//---------------------------------------+
	// 四角形
	m_quadVA = new VertexArray();
	m_quadVA->CreateSimpleQuadVAO();

	//---------------------------------------+
	// 描画バッファ生成
	//---------------------------------------+
	CreateMSAA();

	// ブルーム効果クラスの生成
	m_bloomRender = new RenderBloom;

	// 描画方法の設定
	if (GAME_CONFIG.GetEnableDeferred())
	{
		m_renderMethodType = RENDER_METHOD::DEFERRED;
	}

	// シェーダーマネージャー
	m_shaderManager = new ShaderManager();
	if (!m_shaderManager->CreateShaders())
	{
		std::cout << "Error::ShaderManager CreateShaders()" << std::endl;
		return false;
	}

	//---------------------------------------+
    // uniformバッファ生成
    //---------------------------------------+
    // ビュー行列・プロジェクション行列UBO
	glGenBuffers(1, &m_uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// カメラ情報UBO
	glGenBuffers(1, &m_uboCamera);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboCamera);
	glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_uboCamera);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// トリガーUBO
	glGenBuffers(1, &m_uboTriggers);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboTriggers);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(int), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_uboTriggers);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// ディレクショナルライトUBO(vec3→vec4として送信)
	glGenBuffers(1, &m_uboDirLights);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboDirLights);
	auto byte = ((4 * sizeof(float)) * 4) + (1 * sizeof(float));
	glBufferData(GL_UNIFORM_BUFFER, ((4 * sizeof(float)) * 4) + (1 * sizeof(float)), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 3, m_uboDirLights);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	

	// 描画可能オブジェクト管理クラス
	m_drawableObject = new DrawableObjectManager();

	// skybox
	m_skyBox = new CubeMap("Data/Textures/SkyBox/NightCity/");

	// ディレクショナルライト
	m_dirLight = new DirectionalLight;

	// 各種描画メソッドの生成
	m_renderMethods[RENDER_METHOD::FORWARD] = new RenderForward(this);
	if (!m_renderMethods[RENDER_METHOD::FORWARD]->Load())
	{
		return false;
	}

	m_renderMethods[RENDER_METHOD::DEFERRED] = new RenderDeferred(this);
	if (!m_renderMethods[RENDER_METHOD::DEFERRED]->Load())
	{
		return false;
	}

	return true;
}

/// <summary>
/// レンダラー用デバッグオブジェクトの生成
/// </summary>
void Renderer::CreateRendererDebugObject()
{
	m_debugObj = new RendererDebugObject(this);
	DEBUGGER->AddDebugObject(m_debugObj, OBJECT_TAG::SYSTEM);
}

/// <summary>
/// 解放処理
/// </summary>
void Renderer::Delete()
{
	delete m_shaderManager;
	delete m_drawableObject;
	delete m_quadVA;

	delete m_bloomRender;
	delete m_skyBox;

	// 描画メソッド解放処理
	for (auto itr : m_renderMethods)
	{
		delete itr.second;
	}
	m_renderMethods.clear();

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
	glViewport(0, 0, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH());

	// uniformバッファのセット
	SetUniformBuffer();

	// 現在の描画メソッドに応じた描画処理を行う
	m_renderMethods[m_renderMethodType]->Draw(m_shaderManager, m_drawableObject);

	// 新しいカラーバッファを古いバッファと交換し、画面に表示
	glfwSwapBuffers(m_window);
}

/// <summary>
/// ブルーム効果の付与＆描画
/// </summary>
/// <param name="_highBrightBuffer"> 高輝度成分を保存したバッファ </param>
/// <param name="_colorBuffer"> カラーバッファ </param>
void Renderer::BloomPass(unsigned int _highBrightBuffer, unsigned int _colorBuffer)
{
	// カラー・バッファ情報のクリア
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);      // 指定した色値で画面をクリア
	glClear(GL_COLOR_BUFFER_BIT);                 // 画面のカラー・深度・ステンシルバッファをクリア

	// 縮小バッファ計算
	m_bloomRender->DownSampling(_highBrightBuffer, m_shaderManager->GetShader(GLSLshader::BLOOM_DOWNSAMPLING), m_quadVA);
	// ガウスぼかし処理
	m_bloomRender->GaussBlur(m_shaderManager->GetShader(GLSLshader::BLOOM_GAUSSIAN_BLUR), m_quadVA);
	// 最終トーンマッピング & スクリーン出力
	m_bloomRender->DrawBlendBloom(_colorBuffer, m_shaderManager->GetShader(GLSLshader::BLOOM_TONEMAPPING), m_quadVA);
}


/// <summary>
/// マルチサンプリングアンチエイリアシング用バッファの生成
/// </summary>
void Renderer::CreateMSAA()
{
	// MSAAバッファの生成・登録
	glGenFramebuffers(1, &m_msaaBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_msaaBuffer);
	// マルチサンプル用カラーバッファを生成
	glGenTextures(1, &m_msaaColor);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_msaaColor);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(), GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_msaaColor, 0);
	// MSAA用レンダーバッファの生成 (深度・ステンシルとして登録)
	glGenRenderbuffers(1, &m_msaaRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_msaaRBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH());
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_msaaRBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::MSAA BUFFER::Create Failed" << std::endl;
	}
	
	// MSAAバッファのバインドを解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

/// <summary>
/// uniformバッファへの変数セット関数
/// </summary>
void Renderer::SetUniformBuffer()
{
	// 行列UBO
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(m_viewMat));
	glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_projMat));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// カメラUBO
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboCamera);
	glm::vec3 viewPos = glm::vec3(m_viewMat[3][0], m_viewMat[3][1], m_viewMat[3][2]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float) * 4, glm::value_ptr(glm::vec4(viewPos, 0.0f)));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// トリガーFBO
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboTriggers);
	int bloom = static_cast<int>(GAME_CONFIG.GetEnableBloom());
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(int), &bloom);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// ディレクショナルライト
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboDirLights);
	// 送信時のストライド(シェーダー側ではvec4型として受け取り ※メモリ読み取りがうまくいかないため)
	auto stride = sizeof(float) * 4;
	glBufferSubData(GL_UNIFORM_BUFFER, 0, stride, glm::value_ptr(glm::vec4(m_dirLight->GetDirection(), 0.0f)));
	glBufferSubData(GL_UNIFORM_BUFFER, stride, stride, glm::value_ptr(glm::vec4(m_dirLight->GetDiffuse(), 0.0f)));
	glBufferSubData(GL_UNIFORM_BUFFER, stride * 2, stride, glm::value_ptr(glm::vec4(m_dirLight->GetSpecular(), 0.0f)));
	glBufferSubData(GL_UNIFORM_BUFFER, stride * 3, stride, glm::value_ptr(glm::vec4(m_dirLight->GetAmbient(), 0.0f)));
	glBufferSubData(GL_UNIFORM_BUFFER, stride * 4, sizeof(float), &m_dirLight->GetIntensity());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

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
