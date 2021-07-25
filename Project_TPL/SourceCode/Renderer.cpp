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
#include <gtc/type_ptr.hpp>
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
    // SDLによるGL初期設定
    //--------------------------------------+
	// OpenGL アトリビュートのセット
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// GL version 4.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	// 8Bit RGBA チャンネル
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// ダブルバッファリング有効化
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// ハードウェアアクセラレーションを強制する
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);



	//--------------------------------------+
	// ウィンドウオブジェクト定義
	//--------------------------------------+
	// Windowの作成
	m_window = SDL_CreateWindow
	(
		"Project_TPL",
		0,
		0,
		_width, 
		_height, 
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);

	if (!m_window)
	{
		printf("Windowの作成に失敗: %s", SDL_GetError());
		return false;
	}
	if (_fullScreen)
	{
		if (SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP))
		{
			printf("(%d, %d) サイズのフルスクリーン化に失敗\n", _width, _height);
			return false;
		}
		glViewport(0, 0, _width, _height);
	}

	// OpenGLContextの作成
	m_context = SDL_GL_CreateContext(m_window);
	SDL_GL_MakeCurrent(m_window, m_context);

	//SDLRendererの作成
	m_sdlRenderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!m_sdlRenderer)
	{
		printf("SDLRendererの作成に失敗 : %s", SDL_GetError());
		return false;
	}
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		printf("SDLImageInitPNGの初期化に失敗 : %s", SDL_GetError());
		return false;
	}


	//--------------------------------------+
	// GLEW初期化
	//--------------------------------------+
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error::GLEW Initialize" << std::endl;
		return false;
	}
	// 幾つかのプラットホームでは、GLEWが無害なエラーコードを吐くのでクリアしておく
	glGetError();


	//---------------------------------------+
	// ビューポートの設定
	//---------------------------------------+
	// ビューポート (0x0の座標から1920x1080までを描画範囲として設定)
	glViewport(0, 0, _width, _height);
	// カリングの設定
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	//---------------------------------------+
    // 行列の初期化
    //---------------------------------------+
	m_viewMat = Matrix4::CreateLookAt(Vector3(0.0f, 0.0f, -20.0f), Vector3::Zero, Vector3::UnitZ);
	m_projMat = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
		static_cast<float>(_width),
		static_cast<float>(_height),
		1.0f, 5000.0f);

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
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(Matrix4::mat), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// カメラ情報UBO
	glGenBuffers(1, &m_uboCamera);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboCamera);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Vector3::x) + sizeof(Vector3::y) + sizeof(Vector3::z), NULL, GL_DYNAMIC_DRAW);
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
	glBufferData(GL_UNIFORM_BUFFER, (4 * (sizeof(Vector3::x) + sizeof(Vector3::y) + sizeof(Vector3::z))) + (1 * sizeof(float)), NULL, GL_DYNAMIC_DRAW);
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

	// SDLのクリーンアップ
	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_window);
}

/// <summary>
/// 描画関数
/// </summary>
void Renderer::Draw()
{
	// コンテキストをメイン画面と再リンク
	SDL_GL_MakeCurrent(m_window, m_context);
	
	// ビューポートの更新
	glViewport(0, 0, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH());

	// uniformバッファのセット
	SetUniformBuffer();

	// 現在の描画メソッドに応じた描画処理を行う
	m_renderMethods[m_renderMethodType]->Draw(m_shaderManager, m_drawableObject);

	// 画面のスワップ
	SDL_GL_SwapWindow(m_window);
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
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4::mat), m_viewMat.GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(Matrix4::mat), sizeof(Matrix4::mat), m_projMat.GetAsFloatPtr());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// カメラUBO
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboCamera);
	Vector3 viewPos = m_viewMat.GetTranslation();
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float) * 4, viewPos.GetAsFloatPtr());
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
	glBufferSubData(GL_UNIFORM_BUFFER, 0, stride, m_dirLight->GetDirection().GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, stride, stride, m_dirLight->GetDiffuse().GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, stride * 2, stride, m_dirLight->GetSpecular().GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, stride * 3, stride, m_dirLight->GetAmbient().GetAsFloatPtr());
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
void Renderer::FrameBuffer_Size_Callback(SDL_Window* _window, int _width, int _height)
{
	// ビューポートのリサイズ
	glViewport(0, 0, _width, _height);
}
