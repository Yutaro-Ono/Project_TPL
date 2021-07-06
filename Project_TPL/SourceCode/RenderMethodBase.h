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
#include <iostream>
#include <string>

class RenderMethodBase
{
public:

	RenderMethodBase(class Renderer* _renderer);
	virtual ~RenderMethodBase();

	virtual bool Load() = 0;

	virtual void Draw(class ShaderManager* _shaderManager, class DrawableObjectManager* _drawObjects) = 0;

protected:

	class Renderer* m_renderer;

};