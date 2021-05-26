//----------------------------------------------------------------------------------+
// @file        DebugObjectBase.h
// @brief       �f�o�b�O�I�u�W�F�N�g�x�[�X�N���X
// @note        imgui�ɂ��f�o�b�O�n�̊��N���X
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/23   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"


class DebugObjectBase
{

public:

	DebugObjectBase();
	virtual~DebugObjectBase();

	virtual void Update(float _deltaTime) = 0;

	virtual void Render() = 0;


protected:

	bool m_isShowDebug;

};
