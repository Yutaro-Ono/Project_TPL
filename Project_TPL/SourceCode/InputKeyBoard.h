//----------------------------------------------------------------------------------+
// @file        InputKeyBoard.h
// @brief       �L�[�{�[�h����
// @note        �V���O���g��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 6/23   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SDL.h>


class InputKeyBoard
{

public:

	// �L�[�̉������
	enum KEY_STATE
	{
		KEY_STATE_OFF = 0,          // ������Ă��Ȃ�
		KEY_STATE_PUSHDOWN,     // ��������
		KEY_STATE_PRESSED,      // �����������Ă���
		KEY_STATE_PULLUP        // ������Ԃ��痣����
	};



	// �V���O���g���p�^�[��
	static InputKeyBoard& InputInstance()
	{
		static InputKeyBoard InputInstance;
		return InputInstance;
	}


	~InputKeyBoard() {};

	void Update();

	char GetInput(Uint8 SDL_SCANCODE_key);             // �w�肵���L�[�̓��͏�Ԃ̎擾

	bool IsKeyPressed(Uint8 SDL_SCANCODE_key);         // �w�肵���L�[�͉����ꑱ���Ă����Ԃ����擾
	bool IsKeyPullUp(Uint8 SDL_SCANCODE_key);          // �w�肵���L�[�̃{�^���������ꂽ���H
	bool IsKeyPushDown(Uint8 SDL_SCANCODE_key);        // �w�肵���L�[�͉����ꑱ���Ă��邩
	bool IsKeyOff(Uint8 SDL_SCANCODE_key);             // ����L�[�͉�����Ă��Ȃ���


private:

	InputKeyBoard();

	char m_key[256];
	char m_keyState[2][256];

	int m_nowKey;
	int m_prevKey;

};

#define INPUT_KEYBOARD_INSTANCE InputKeyBoard::InputInstance()