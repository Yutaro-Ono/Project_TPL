//----------------------------------------------------------------------------------+
// @file        InputKeyBoard.h
// @brief       キーボード入力
// @note        シングルトン
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 6/23   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SDL.h>


class InputKeyBoard
{

public:

	// キーの押下状態
	enum KEY_STATE
	{
		KEY_STATE_OFF = 0,          // 押されていない
		KEY_STATE_PUSHDOWN,     // 押下した
		KEY_STATE_PRESSED,      // 押下し続けている
		KEY_STATE_PULLUP        // 押下状態から離した
	};



	// シングルトンパターン
	static InputKeyBoard& InputInstance()
	{
		static InputKeyBoard InputInstance;
		return InputInstance;
	}


	~InputKeyBoard() {};

	void Update();

	char GetInput(Uint8 SDL_SCANCODE_key);             // 指定したキーの入力状態の取得

	bool IsKeyPressed(Uint8 SDL_SCANCODE_key);         // 指定したキーは押され続けている状態かを取得
	bool IsKeyPullUp(Uint8 SDL_SCANCODE_key);          // 指定したキーのボタンが離されたか？
	bool IsKeyPushDown(Uint8 SDL_SCANCODE_key);        // 指定したキーは押され続けているか
	bool IsKeyOff(Uint8 SDL_SCANCODE_key);             // あるキーは押されていないか


private:

	InputKeyBoard();

	char m_key[256];
	char m_keyState[2][256];

	int m_nowKey;
	int m_prevKey;

};

#define INPUT_KEYBOARD_INSTANCE InputKeyBoard::InputInstance()