//----------------------------------------------------------------------------------+
// @file        InputKeyBoard.cpp
// @brief       �L�[�{�[�h����
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 6/23   �V�K�쐬
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
	// ���݂̃L�[��1�O�̃L�[�̏�Ԃ����ւ���  0���P�ɁA1��0��
	m_nowKey ^= 1;
	m_prevKey = m_nowKey ^ 1;

	//�L�[��Ԏ擾
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	memcpy(m_keyState[m_nowKey], keys, sizeof(char) * 256);

	for (int i = 0; i < 256; i++)
	{
		char nowInput = m_keyState[m_nowKey][i];
		char prevInput = m_keyState[m_prevKey][i];

		if (nowInput) //���݉�����Ă���
		{
			//�O�̃t���[����������Ă����H
			if (prevInput)
			{
				m_key[i] = KEY_STATE_PRESSED; // ��������ςȂ�
			}
			else {
				m_key[i] = KEY_STATE_PUSHDOWN;// �������ꂽ
			}

		}
		else // ���݃L�[�͉�����Ă��Ȃ�
		{
			// �O�̃t���[���ŉ�����Ă����H
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

