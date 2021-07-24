//----------------------------------------------------------------------------------+
// @file        InputKeyBoard.cpp
// @brief       キーボード入力
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 6/23   新規作成
//----------------------------------------------------------------------------------+
#include "InputKeyBoard.h"
#include <memory.h>

InputKeyBoard::InputKeyBoard()
    :m_nowKey(0)
    ,m_prevKey(1)
{
    for (int i = 0; i < 256; i++)
    {
        m_key[i] = 0;
        m_keyState[m_nowKey][i] = KEY_STATE::KEY_STATE_OFF;
        m_keyState[m_prevKey][i] = KEY_STATE::KEY_STATE_OFF;
    }
}

void InputKeyBoard::Update()
{
	// 現在のキーと1つ前のキーの状態を入れ替える  0を１に、1を0に
	m_nowKey ^= 1;
	m_prevKey = m_nowKey ^ 1;

	//キー状態取得
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	memcpy(m_keyState[m_nowKey], keys, sizeof(char) * 256);

	for (int i = 0; i < 256; i++)
	{
		char nowInput = m_keyState[m_nowKey][i];
		char prevInput = m_keyState[m_prevKey][i];

		if (nowInput) //現在押されている
		{
			//前のフレームも押されていた？
			if (prevInput)
			{
				m_key[i] = KEY_STATE_PRESSED; // 押されっぱなし
			}
			else {
				m_key[i] = KEY_STATE_PUSHDOWN;// 今押された
			}

		}
		else // 現在キーは押されていない
		{
			// 前のフレームで押されていた？
			if (prevInput)
			{
				m_key[i] = KEY_STATE_PULLUP;
			}
			else {
				m_key[i] = KEY_STATE_OFF;
			}
		}
	}
}

char InputKeyBoard::GetInput(Uint8 SDL_SCANCODE_key)
{
	return m_key[SDL_SCANCODE_key];
}

bool InputKeyBoard::IsKeyPressed(Uint8 SDL_SCANCODE_key)
{
	return m_key[SDL_SCANCODE_key] == KEY_STATE_PRESSED;
}

bool InputKeyBoard::IsKeyPullUp(Uint8 SDL_SCANCODE_key)
{
	return m_key[SDL_SCANCODE_key] == KEY_STATE_PULLUP;
}

bool InputKeyBoard::IsKeyPushDown(Uint8 SDL_SCANCODE_key)
{
	return m_key[SDL_SCANCODE_key] == KEY_STATE_PUSHDOWN;
}

bool InputKeyBoard::IsKeyOff(Uint8 SDL_SCANCODE_key)
{
	return m_key[SDL_SCANCODE_key] == KEY_STATE_OFF;
}

