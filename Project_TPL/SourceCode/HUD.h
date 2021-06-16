//----------------------------------------------------------------------------------+
// @file        HUD.h
// @brief       HUD(UI)のベースクラス
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 6/15   新規作成
//----------------------------------------------------------------------------------+
#pragma once

/// <summary>
/// HUDの状態 (CLOSEで消去)
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


	bool m_isActive;         // HUDの表示・非表示


};