//----------------------------------------------------------------------------------+
// @file        TestSphere.h
// @brief       �e�X�g���̃N���X
// @note        �A�N�^�[
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/19   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "Actor.h"


class TestSphere : public Actor
{

public:

	TestSphere();
	~TestSphere();

	void UpdateActor(float _deltaTime) override;

private:




};