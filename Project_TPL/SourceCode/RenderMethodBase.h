//----------------------------------------------------------------------------------+
// @file        RenderMethodBase.h
// @brief       描画方式のベースクラス
//              
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/22   新規作成
// 2021/ 5/26   各種バッファを登録 (Gバッファ・ライト・MSAA)
//----------------------------------------------------------------------------------+
#pragma once
#include "Renderer.h"
#include <iostream>
#include <string>

class RenderMethodBase
{
public:

	RenderMethodBase(class Renderer* _owner, RENDER_METHOD _method);
	virtual ~RenderMethodBase() = 0;

	virtual bool Load() = 0;

	virtual bool Draw() = 0;

protected:

	class Renderer* m_owner;

	RENDER_METHOD m_method;

};