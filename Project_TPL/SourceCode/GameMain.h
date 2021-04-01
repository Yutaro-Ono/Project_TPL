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


private:

	GameMain();
	
	void Delete();
	void ProcessInput();


	// メンバ インスタンス
	class Renderer* m_renderer;     // レンダラー
	class Debugger* m_debugger;     // デバッガ―
	class SceneBase* m_scene;       // ゲームシーン

};

#define GAME_INSTANCE GameMain::GetInstance()