//-------------------------------------------------------------+
// @project     THE PARADISE LOST.
// @overview    OpenGL/C++で製作するゲームプロジェクト
//              三人称視点のゾンビシューターになる予定    
// 
// @file        main.cpp
// @brief       メインループ
// @note        詳細はGameMainクラスで定義。
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/21   imgui導入
// 2021/ 3/19   プロジェクト開始。
//
//-------------------------------------------------------------+
// インクルードファイル
#include "GameMain.h"

int main()
{

	bool loop = GAME_INSTANCE.Initialize();

	// メインループ
	while (loop)
	{
		loop = GAME_INSTANCE.RunLoop();
	}

	return 0;
}