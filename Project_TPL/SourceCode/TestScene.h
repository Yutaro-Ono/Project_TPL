//----------------------------------------------------------------------------------+
// @file        SceneBase.h
// @brief       �e�X�g�V�[��
// @note        SceneBase�N���X���p��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/24   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "SceneBase.h"


class TestScene : public SceneBase
{
public:

	TestScene();
	~TestScene();

	void Initialize() override;

	SceneBase* Update(float _deltaTime) override;

private:






};