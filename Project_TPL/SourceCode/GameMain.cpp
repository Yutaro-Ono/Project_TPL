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
#include "InputKeyBoard.h"
#include <iostream>


/// <summary>
/// コンストラクタ
/// </summary>
GameMain::GameMain()
	:m_isRunning(true)
	,m_texturePool(nullptr)
	,m_meshPool(nullptr)
	,m_actorPool(nullptr)
	,m_renderer(nullptr)
	,m_debugger(nullptr)
	,m_scene(nullptr)
	,m_deltaTime(0.0f)
	,m_ticksCount(0)
	,m_bulletTime(0.0f)
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
	if (!GAME_CONFIG.Load("Project_TPL.ini"))
	{
		std::cout << "Error::GameSettings::Load" << std::endl;
		return false;
	}

	// SDL初期化
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf("SDL初期化失敗: %s\n", SDL_GetError());
		return false;
	}
	// サウンドの初期化
	if (!Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG))
	{
		return false;
	}
	printf("SDLMixer初期化完了\n");
	// SDLMixer API初期化　44100:音源の周波数 2:ステレオ 4096:内部バッファサイズ
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		Mix_CloseAudio();
		Mix_Quit();
		return false;
	}
	int decordNum = Mix_GetNumMusicDecoders();
	for (int i = 0; i < decordNum; ++i)
	{
		printf("MusicDecorder %d : %s\n", i, Mix_GetMusicDecoder(i));
	}
	int chunkNum = Mix_GetNumChunkDecoders();
	for (int i = 0; i < chunkNum; ++i)
	{
		printf("SoundDecorder %d : %s\n", i, Mix_GetChunkDecoder(i));
	}
	printf("SDLMixerAPI初期化完了\n");



	// 各種オブジェクトプールの生成
	m_texturePool = new TexturePool();
	m_meshPool = new MeshPool();
	m_actorPool = new ActorPool();

	// レンダラークラスの作成
	m_renderer = new Renderer();
	if (!m_renderer->Initialize(GAME_CONFIG.m_displayWidth, GAME_CONFIG.GetInstance().m_displayHeight, GAME_CONFIG.m_displayFullScreen))
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
	// レンダラー関連デバッグオブジェクトの生成
	m_renderer->CreateRendererDebugObject();

#endif

	// レンダラークラスのロード処理 ※デバッガーを生成した後
	if (!m_renderer->Load())
	{
		std::cout << "Error::Renderer::Load" << std::endl;
		return false;
	}

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
	// 終了イベントのキャッチ
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{

		switch (event.type)
		{
		case SDL_QUIT:
			m_isRunning = false;
			break;

		default:
			//CONTROLLER_INSTANCE.ReceiveEvent(event);
			break;
		}
	}

	// キーボード入力更新
	INPUT_KEYBOARD_INSTANCE.Update();

	// コントローラ入力更新
	//CONTROLLER_INSTANCE.Update();

	// マウス入力更新
	//MOUSE_INSTANCE.Update();

	// ESCが押されたら終了
	if (INPUT_KEYBOARD_INSTANCE.IsKeyPullUp(SDL_SCANCODE_ESCAPE))
	{
		m_isRunning = false;
	}


	// アクターデバッグ
	//if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_F12))
	//{
	//	ShowActor();
	//}

	// レンダリングリソース表示
	//if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_F11))
	//{
	//	GetRenderer()->ShowResource();
	//}

	// ポーズモード移行／解除
	//if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_BACKSPACE) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_START))
	//{
	//	//ポーズモード切替
	//	m_isPauseMode = !m_isPauseMode;
	//	Actor::State changeState;
	//	if (m_isPauseMode)
	//	{
	//		changeState = Actor::STATE_PAUSED;        // ポーズ
	//		m_pause->SetModeON();                     // ポーズ画面をオン
	//	}
	//	else
	//	{
	//		m_pause->SetModeOFF();                    // ポーズ画面をオフ
	//		changeState = Actor::STATE_ACTIVE;        // アクティブ
	//	}

	//	//全てのステートを変更する
	//	for (auto itr : m_actors)
	//	{
	//		if (itr->GetState() != Actor::STATE_DEAD)
	//		{
	//			itr->SetState(changeState);
	//		}
	//	}
	//}

#ifdef _DEBUG

	//------------------------------------------------+
	// イベントの更新
	//------------------------------------------------+

#endif
}

/// <summary>
/// 全処理のループ関数
/// </summary>
/// <returns> ループ中で問題が発生した場合falseを返す </returns>
bool GameMain::RunLoop()
{
	//------------------------------------------------+
	// メインループ
	//------------------------------------------------+
	// 入力処理
	ProcessInput();

	// デルタタイム更新
	UpdateDeltaTime();

	// シーンの更新
	SceneBase* currentScene = m_scene->Update(m_deltaTime);
	// 現在シーンが更新されていたら
	if (currentScene != m_scene)
	{
		// 前のシーンを削除
		delete m_scene;
		// メインシーンを更新されたシーンに切り替え・初期化
		m_scene = currentScene;
		m_scene->Initialize();
	}

	// アクターの更新
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

#ifdef _DEBUG

	//------------------------------------------------+
	// imgui描画処理 (Debugビルドでのみ行う)
	//------------------------------------------------+
	m_debugger->RenderImGui();

#endif
	


	return m_isRunning;
	
}


/// <summary>
/// フレーム関係の更新処理
/// </summary>
void GameMain::UpdateDeltaTime()
{
	// 16ミリ秒(= 60フレーム/秒)になるように、前のフレームからの16ミリ秒以上経過するまで待つ
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_ticksCount + 16))
		;

	// 前のフレームから現在時刻までの経過時間算出(秒単位）
	m_deltaTime = (SDL_GetTicks() - m_ticksCount + 16) / 1000.0f;

	//このフレームの開始時間保存（次のフレームまでの経過時間をはかるため）
	m_ticksCount = SDL_GetTicks();

	// フレーム時間があまりにも経過している場合は0.05 → 20fps固定
	if (m_deltaTime > 0.05f)
	{
		m_deltaTime = 0.05f;
	}
}

/// <summary>
/// シーンのセット
/// </summary>
/// <param name="_scene"> シーンベースクラス </param>
void GameMain::SetScene(SceneBase* _scene)
{
	if (m_scene == nullptr)
	{
		m_scene = _scene;
	}
	else
	{
		delete m_scene;
		m_scene = _scene;
	}

	m_scene->Initialize();
}