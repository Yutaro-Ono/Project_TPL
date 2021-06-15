//-------------------------------------------------------------+
// @project     THE PARADISE LOST.
// @overview    OpenGL/C++�Ő��삷��Q�[���v���W�F�N�g
//              �O�l�̎��_�̃]���r�V���[�^�[�ɂȂ�\��    
// 
// @file        main.cpp
// @brief       ���C�����[�v
// @note        �ڍׂ�GameMain�N���X�Œ�`�B
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/21   imgui����
// 2021/ 3/19   �v���W�F�N�g�J�n�B
//
//-------------------------------------------------------------+
// �C���N���[�h�t�@�C��
#include "GameMain.h"
#include "TestScene.h"

int main()
{
	// �������E�����V�[���̃Z�b�g
	bool loop = GAME_INSTANCE.Initialize();
	GAME_INSTANCE.SetScene(new TestScene());

	// ���C�����[�v
	while (loop)
	{
		loop = GAME_INSTANCE.RunLoop();
	}

	return 0;
}