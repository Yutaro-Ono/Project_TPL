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

InputKeyBoard::~InputKeyBoard()
{
}

void InputKeyBoard::Update()
{
	//// ���݂̃L�[��1�O�̃L�[�̏�Ԃ����ւ���  0���P�ɁA1��0��
	//m_nowKey ^= 1;
	//m_prevKey = m_nowKey ^ 1;

	////�L�[��Ԏ擾
	//const UINT* keys = glfwGetKey(NULL);
	//memcpy(m_keyState[m_nowKey], keys, sizeof(char) * 256);

	//for (int i = 0; i < 256; i++)
	//{
	//	char nowInput = m_keyState[m_nowKey][i];
	//	char prevInput = m_keyState[m_prevKey][i];

	//	if (nowInput) //���݉�����Ă���
	//	{
	//		//�O�̃t���[����������Ă����H
	//		if (prevInput)
	//		{
	//			m_key[i] = KEY_STATE_PRESSED; // ��������ςȂ�
	//		}
	//		else {
	//			m_key[i] = KEY_STATE_PUSHDOWN;// �������ꂽ
	//		}

	//	}
	//	else // ���݃L�[�͉�����Ă��Ȃ�
	//	{
	//		// �O�̃t���[���ŉ�����Ă����H
	//		if (prevInput)
	//		{
	//			m_key[i] = KEY_STATE_PULLUP;
	//		}
	//		else {
	//			m_key[i] = KEY_STATE_OFF;
	//		}
	//	}
	//}
}

char InputKeyBoard::GetInput(UINT _GLFW_KEYCODE)
{
    return m_key[_GLFW_KEYCODE];
}

bool InputKeyBoard::IsKeyPressed(UINT _GLFW_KEYCODE)
{
    return false;
}

bool InputKeyBoard::IsKeyPullUp(UINT _GLFW_KEYCODE)
{
    return false;
}

bool InputKeyBoard::IsKeyPushDown(UINT _GLFW_KEYCODE)
{
    return false;
}

bool InputKeyBoard::IsKeyOff(UINT _GLFW_KEYCODE)
{
    return false;
}
