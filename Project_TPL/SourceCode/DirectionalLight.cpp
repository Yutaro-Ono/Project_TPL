//----------------------------------------------------------------------------------+
// @file        DirectionalLight.cpp
// @brief       �f�B���N�V���i�����C�g�N���X
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/26   �V�K�쐬
//----------------------------------------------------------------------------------+
#include "DirectionalLight.h"
#include <string>
#include "GameMain.h"
#include "Debugger.h"

DirectionalLight::DirectionalLight()
	:m_lightColor(glm::vec3(1.0f))
	,m_lightDirection(glm::vec3(0.0f, -1.0f, 0.0f))
	,m_intensity(1.0f)
	,m_debugObj(nullptr)
{
	// �f�o�b�O�I�u�W�F�N�g�̐���
#ifdef _DEBUG

	m_debugObj = new DirectionalLightDebug(this);
	DEBUGGER->AddDebugObject(m_debugObj, OBJECT_TAG::SYSTEM);

#endif
}

DirectionalLight::~DirectionalLight()
{
}





DirectionalLightDebug::DirectionalLightDebug(class DirectionalLight* _owner)
	:m_owner(_owner)
	,m_colorPick(false)
{
}

DirectionalLightDebug::~DirectionalLightDebug()
{
}

void DirectionalLightDebug::Update(float _deltaTime)
{
	// ���x��
	std::string label = "Directional Light";

	// ��؂��
	ImGui::Separator();

	if (ImGui::TreeNode(label.c_str()))
	{
		//------------------------------------------------------------------------+
		// ���̃J���[
		ImGui::Text("Directional Light Color");
		//glm::vec3 color;
		static float color[3];
		color[0] = m_owner->m_lightColor.x;
		color[1] = m_owner->m_lightColor.y;
		color[2] = m_owner->m_lightColor.z;

		label = "Directional Light Color : R";
		ImGui::SliderFloat(label.c_str(), &color[0], 0.0f, 1.0f);
		label = "Directional Light Color : G";
		ImGui::SliderFloat(label.c_str(), &color[1], 0.0f, 1.0f);
		label = "Directional Light Color : B";
		ImGui::SliderFloat(label.c_str(), &color[2], 0.0f, 1.0f);

		// �J���[�s�b�J�[���g�p���邩
		ImGui::Checkbox("Color Picker", &m_colorPick);
		if (m_colorPick)
		{
			ImGui::ColorPicker3("Directional Light Color Picker", color);
		}

		// �X�V���ꂽ��
		if (color[0] != m_owner->m_lightColor.x || color[1] != m_owner->m_lightColor.y || color[2] != m_owner->m_lightColor.z)
		{
			m_owner->SetLightColor(glm::vec3(color[0], color[1], color[2]));
		}
		//--------------------------------------------------------------------------+

		ImGui::TreePop();
	}
}

void DirectionalLightDebug::Render()
{
}
