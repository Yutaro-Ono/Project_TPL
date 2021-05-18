//----------------------------------------------------------------------------------+
// @file        GameMain.h
// @brief       ゲームループに関わるインスタンスを生成・保持
//              更新処理も行う
// @note        シングルトン化し、includeすればどこからでもアクセス可能
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/21   新規作成
//
//----------------------------------------------------------------------------------+
#pragma once
// インクルードファイル
// ※gl3w→glfwの順番に記述
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>


class GameMain
{

public:

	// シングルトンパターン
	// GameMainクラスのローカル静的オブジェクトを返す
	static GameMain& GetInstance()
	{
		static GameMain game;
		return game;
	}

	~GameMain();

	bool Initialize();
	bool RunLoop();

	// Getter
	class Renderer* GetRenderer() { return m_renderer; }
	class TexturePool* GetTexturePool() { return m_texturePool; }
	class MeshPool* GetMeshPool() { return m_meshPool; }
	class ActorPool* GetActorPool() { return m_actorPool; }

private:

	GameMain();
	
	void Delete();
	void ProcessInput();


	// メンバ クラス
	class GameSettings* m_settings;        // ゲーム設定
	class Renderer* m_renderer;            // レンダラー
	class Debugger* m_debugger;            // デバッガ―
	class SceneBase* m_scene;              // ゲームシーン

	class TexturePool* m_texturePool;      // テクスチャプール
	class MeshPool* m_meshPool;            // メッシュプール
	class ActorPool* m_actorPool;          // アクタープール

};

#define GAME_INSTANCE GameMain::GetInstance()
#define RENDERER GameMain::GetInstance().GetRenderer()
#define MESH_POOL GameMain::GetInstance().GetMeshPool()