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

/// <summary>
/// コンストラクタ
/// </summary>
Renderer::Renderer()
	:m_window(NULL)
	,m_renderMethod(RENDER_METHOD::DEFFERED)
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
	CreateGBuffer();
	CreateLightBuffer();
	CreateMSAA();

	// ブルーム効果クラスの生成
	m_bloomRender = new RenderBloom;

	// 描画方法の設定
	if (GAME_CONFIG.GetEnableDeffered())
	{
		m_renderMethod = RENDER_METHOD::DEFFERED;
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

	//------------------------------------------------+
	// ForwardShading
	//------------------------------------------------+
	if (m_renderMethod == RENDER_METHOD::FORWARD)
	{

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);      // 指定した色値で画面をクリア
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);              // 画面のカラー・深度・ステンシルバッファをクリア

		// メッシュの描画
		glEnable(GL_DEPTH_TEST);
		m_shaderManager->EnableShaderProgram(GLSLshader::BASIC_MESH);
		m_drawableObject->Draw(m_shaderManager, GLSLshader::BASIC_MESH);

		// 法線の視覚化
		if (m_visualizeNormal)
		{
			m_shaderManager->EnableShaderProgram(GLSLshader::OPTION_NORMAL_VISUALIZE);
			m_drawableObject->Draw(m_shaderManager, GLSLshader::OPTION_NORMAL_VISUALIZE);
		}
	}

	//------------------------------------------------+
	// DefferedShading
	//------------------------------------------------+
	if (m_renderMethod == RENDER_METHOD::DEFFERED)
	{
		// Gバッファをバインド
		glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

		// カラー・バッファ情報のクリア
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);      // 指定した色値で画面をクリア
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);              // 画面のカラー・深度・ステンシルバッファをクリア
		glEnable(GL_DEPTH_TEST);

		//-------------------------------------------------------------------------
		// 0.ジオメトリパス
		//-------------------------------------------------------------------------
		// SkyBox
		m_shaderManager->EnableShaderProgram(GLSLshader::GBUFFER_BASIC_SKYBOX);
		glm::mat4 remView = glm::mat4(glm::mat3(m_viewMat));
		m_shaderManager->GetShader(GLSLshader::GBUFFER_BASIC_SKYBOX)->SetUniform("u_removeTransView", remView);
		m_skyBox->Draw(m_shaderManager->GetShader(GLSLshader::GBUFFER_BASIC_SKYBOX));

		// Mesh
		glEnable(GL_DEPTH_TEST);
		m_shaderManager->EnableShaderProgram(GLSLshader::GBUFFER_BASIC_MESH);
		m_shaderManager->GetShader(GLSLshader::GBUFFER_BASIC_MESH)->SetUniform("u_mat.albedo", 0);
		m_shaderManager->GetShader(GLSLshader::GBUFFER_BASIC_MESH)->SetUniform("u_mat.specular", 5);
		m_shaderManager->GetShader(GLSLshader::GBUFFER_BASIC_MESH)->SetUniform("u_mat.emissive", 6);
		m_drawableObject->Draw(m_shaderManager, GLSLshader::GBUFFER_BASIC_MESH);

		// Phongシェーディング
		//m_shaderManager->EnableShaderProgram(GLSLshader::GBUFFER_PHONG);
		//m_shaderManager->GetShader(GLSLshader::GBUFFER_PHONG)->SetUniform("u_mat.albedo", 0);
		//m_shaderManager->GetShader(GLSLshader::GBUFFER_PHONG)->SetUniform("u_mat.specular", 5);
		//m_shaderManager->GetShader(GLSLshader::GBUFFER_PHONG)->SetUniform("u_mat.emissive", 6);
		//m_drawableObject->Draw(m_shaderManager, GLSLshader::GBUFFER_PHONG);

		// 法線の視覚化
		if (m_visualizeNormal)
		{
			glEnable(GL_DEPTH_TEST);
			m_shaderManager->EnableShaderProgram(GLSLshader::OPTION_NORMAL_VISUALIZE_GBUFFER);
			m_drawableObject->Draw(m_shaderManager, GLSLshader::OPTION_NORMAL_VISUALIZE_GBUFFER);
		}

		// Gバッファバインド解除
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		//----------------------------------------------------------------------------+
		// ライティングパス
		//----------------------------------------------------------------------------+
		glBindFramebuffer(GL_FRAMEBUFFER, m_lightBuffer);
		// ブレンドの有効化
		glEnablei(GL_BLEND, 0);
		glBlendFuncSeparatei(0, GL_ONE, GL_ONE, GL_ONE, GL_ONE);     // 加算合成
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		// カリングの有効化
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glFrontFace(GL_CCW);
		// GBufferをテクスチャとしてバインド
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_gPos);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_gNormal);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_gEmissive);

		//-------------------------------------------------------------------+
        // Point Light

		//-------------------------------------------------------------------+
		// Spot Light

		
		glDisable(GL_CULL_FACE);

		//-------------------------------------------------------------------+
		// Directional Light
		m_shaderManager->EnableShaderProgram(GLSLshader::DIRECTIONAL_LIGHT_PASS);
		m_shaderManager->GetShader(GLSLshader::DIRECTIONAL_LIGHT_PASS)->SetUniform("u_gBuffer.position", 0);
		m_shaderManager->GetShader(GLSLshader::DIRECTIONAL_LIGHT_PASS)->SetUniform("u_gBuffer.normal", 1);
		m_shaderManager->GetShader(GLSLshader::DIRECTIONAL_LIGHT_PASS)->SetUniform("u_gBuffer.albedoSpec", 2);
		m_shaderManager->GetShader(GLSLshader::DIRECTIONAL_LIGHT_PASS)->SetUniform("u_gBuffer.emissive", 3);
		m_quadVA->SetActive();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// ライトパス後はブレンドを切る
		glDisable(GL_BLEND);
		glDisablei(GL_BLEND, 0);

		// gBufferの深度情報をライトバッファへコピーする
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer);            // 読み取り先としてGBufferを指定
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_lightBuffer);        // 書き込み先にライトバッファを指定

		glBlitFramebuffer(0, 0, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(),
			              0, 0, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(),
			              GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		glBindFramebuffer(GL_FRAMEBUFFER, m_lightBuffer);


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glEnable(GL_DEPTH_TEST);

		// スクリーンに出力
		// ブルーム処理する
		if (m_enableBloom)
		{
			// カラー・バッファ情報のクリア
			glClearColor(0.05f, 0.05f, 0.05f, 1.0f);      // 指定した色値で画面をクリア
			glClear(GL_COLOR_BUFFER_BIT);                 // 画面のカラー・深度・ステンシルバッファをクリア

			// 縮小バッファ計算
			m_bloomRender->DownSampling(m_lightHighBright, m_shaderManager->GetShader(GLSLshader::BLOOM_DOWNSAMPLING), m_quadVA);
			// ガウスぼかし処理
			m_bloomRender->GaussBlur(m_shaderManager->GetShader(GLSLshader::BLOOM_GAUSSIAN_BLUR), m_quadVA);
			// 最終トーンマッピング & スクリーン出力
			m_bloomRender->DrawBlendBloom(m_lightHDR, m_shaderManager->GetShader(GLSLshader::BLOOM_TONEMAPPING), m_quadVA);
		}
		// ブルーム処理しない
		else
		{
			glDisable(GL_DEPTH_TEST);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_shaderManager->EnableShaderProgram(GLSLshader::OUT_SCREEN_ENTIRE);
			glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);
			glBindTexture(GL_TEXTURE_2D, m_lightHDR);

			// スクリーンを描画
			m_quadVA->SetActive();
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}


	}

	// 新しいカラーバッファを古いバッファと交換し、画面に表示
	glfwSwapBuffers(m_window);
}


/// <summary>
/// Gバッファの各種要素の生成・登録
/// </summary>
void Renderer::CreateGBuffer()
{
	// Gバッファの生成・バインド
	glGenFramebuffers(1, &m_gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
	
	//--------------------------------------------------------------------+
	// Gバッファ各要素の登録
	//--------------------------------------------------------------------+
	// 座標情報バッファ (浮動小数点バッファ / 0番目)
	glGenTextures(1, &m_gPos);
	glBindTexture(GL_TEXTURE_2D, m_gPos);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gPos, 0);
	// 法線情報バッファ (浮動小数点バッファ / 1番目)
	glGenTextures(1, &m_gNormal);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gNormal, 0);
	// アルベド&スペキュラ情報バッファ (8bitカラーバッファ / 2番目)
	glGenTextures(1, &m_gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gAlbedoSpec, 0);
	// 高輝度バッファ (エミッシブバッファ / 3番目)
	glGenTextures(1, &m_gEmissive);
	glBindTexture(GL_TEXTURE_2D, m_gEmissive);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_gEmissive, 0);

	// 生成した各要素をGバッファの描画先としてGLに明示する
	m_gAttachments[0] = { GL_COLOR_ATTACHMENT0 };
	m_gAttachments[1] = { GL_COLOR_ATTACHMENT1 };
	m_gAttachments[2] = { GL_COLOR_ATTACHMENT2 };
	m_gAttachments[3] = { GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, m_gAttachments);

	// レンダーバッファの生成・登録
	glGenRenderbuffers(1, &m_gRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_gRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_gRBO);

	// フレームバッファの整合性チェック
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::GBuffer::Create Failed" << std::endl;
	}

	// Gバッファのバインドを解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

/// <summary>
/// ライトバッファの各種要素の登録
/// </summary>
void Renderer::CreateLightBuffer()
{
	glGenFramebuffers(1, &m_lightBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightBuffer);

	// HDRバッファ
	glGenTextures(1, &m_lightHDR);
	glBindTexture(GL_TEXTURE_2D, m_lightHDR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_lightHDR, 0);
	// 高輝度バッファの作成
	glGenTextures(1, &m_lightHighBright);
	glBindTexture(GL_TEXTURE_2D, m_lightHighBright);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_lightHighBright, 0);
	// アタッチメント設定
	m_lightAttachments[0] = { GL_COLOR_ATTACHMENT0 };
	m_lightAttachments[1] = { GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, m_lightAttachments);

	// レンダーバッファを作成する
	glGenRenderbuffers(1, &m_lightRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_lightRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_lightRBO);

	// フレームバッファの整合性をチェック
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::LightBuffer::Create Failed" << std::endl;
	}
	// フレームバッファのバインド解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
