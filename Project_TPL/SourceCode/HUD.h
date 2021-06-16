//----------------------------------------------------------------------------------+
// @file        HUD.h
// @brief       HUD(UI)�̃x�[�X�N���X
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 6/15   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once

/// <summary>
/// HUD�̏�� (CLOSE�ŏ���)
/// </summary>
enum class HUD_STATE
{
	ACTIVE = 0,
	CLOSE
};


class HUD
{

public:

	HUD();
	virtual ~HUD();




protected:


	bool m_isActive;         // HUD�̕\���E��\��


};