//----------------------------------------------------------------------------------+
// @file        SceneBase.h
// @brief       �V�[���̊��N���X.
// @note        ���ۃN���X
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/24   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once

class SceneBase
{
public:


	SceneBase() {};
	virtual ~SceneBase() {};

	virtual SceneBase* Update(float _deltaTime) = 0;

	virtual void Initialize() = 0;

};