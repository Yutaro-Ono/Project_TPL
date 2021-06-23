//----------------------------------------------------------------------------------+
// @file        InputKeyBoard.h
// @brief       キーボード入力
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 6/23   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>



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


	InputKeyBoard();
	~InputKeyBoard();

	void Update();

	char GetInput(UINT _GLFW_KEYCODE);

	bool IsKeyPressed(UINT _GLFW_KEYCODE);
	bool IsKeyPullUp(UINT _GLFW_KEYCODE);
	bool IsKeyPushDown(UINT _GLFW_KEYCODE);
	bool IsKeyOff(UINT _GLFW_KEYCODE);


private:

	char m_key[256];
	char m_keyState[2][256];

	int m_nowKey;
	int m_prevKey;

};