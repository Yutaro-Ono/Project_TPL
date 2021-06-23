//----------------------------------------------------------------------------------+
// @file        InputKeyBoard.h
// @brief       �L�[�{�[�h����
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 6/23   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>



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