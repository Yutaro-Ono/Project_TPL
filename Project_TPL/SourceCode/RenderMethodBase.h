//----------------------------------------------------------------------------------+
// @file        RenderMethodBase.h
// @brief       �`������̃x�[�X�N���X
//              
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/22   �V�K�쐬
// 2021/ 5/26   �e��o�b�t�@��o�^ (G�o�b�t�@�E���C�g�EMSAA)
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