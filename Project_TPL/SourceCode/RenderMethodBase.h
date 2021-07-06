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