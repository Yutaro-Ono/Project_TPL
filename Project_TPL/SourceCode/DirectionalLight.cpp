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
	DEBUGGER->AddDebugObject(m_debugObj);

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

	// デバッグ画面を表示するかどうか
	ImGui::Checkbox(label.c_str(), &m_isShowDebug);

	if (m_isShowDebug)
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

	}
}

void DirectionalLightDebug::Render()
{
}
