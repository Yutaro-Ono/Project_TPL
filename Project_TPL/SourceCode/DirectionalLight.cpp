//----------------------------------------------------------------------------------+
// @file        DirectionalLight.cpp
// @brief       ディレクショナルライトクラス
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/26   新規作成
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
	// デバッグオブジェクトの生成
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
	// ラベル
	std::string label = "Directional Light";

	// 区切り線
	ImGui::Separator();

	if (ImGui::TreeNode(label.c_str()))
	{
		//------------------------------------------------------------------------+
		// 光のカラー
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

		// カラーピッカーを使用するか
		ImGui::Checkbox("Color Picker", &m_colorPick);
		if (m_colorPick)
		{
			ImGui::ColorPicker3("Directional Light Color Picker", color);
		}

		// 更新されたら
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
