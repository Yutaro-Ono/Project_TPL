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
#include "Renderer.h"

DirectionalLight::DirectionalLight()
	:m_diffuse(glm::vec3(1.0f))
	,m_specular(glm::vec3(0.5f))
	,m_ambient(glm::vec3(0.1f))
	,m_lightDirection(glm::vec3(0.0f, -1.0f, 0.0f))
	,m_intensity(0.5f)
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
		// Diffuse
		ImGui::Text("Directional Light : Diffuse");
		//glm::vec3 color;
		static float diffuse[3];
		diffuse[0] = m_owner->m_diffuse.x;
		diffuse[1] = m_owner->m_diffuse.y;
		diffuse[2] = m_owner->m_diffuse.z;

		label = "Directional Light Color : R";
		ImGui::SliderFloat(label.c_str(), &diffuse[0], 0.0f, 1.0f);
		label = "Directional Light Color : G";
		ImGui::SliderFloat(label.c_str(), &diffuse[1], 0.0f, 1.0f);
		label = "Directional Light Color : B";
		ImGui::SliderFloat(label.c_str(), &diffuse[2], 0.0f, 1.0f);

		// カラーピッカーを使用するか
		ImGui::Checkbox("Color Picker", &m_colorPick);
		if (m_colorPick)
		{
			ImGui::ColorPicker3("Directional Light Color Picker", diffuse);
		}

		// 更新されたら
		if (diffuse[0] != m_owner->m_diffuse.x || diffuse[1] != m_owner->m_diffuse.y || diffuse[2] != m_owner->m_diffuse.z)
		{
			m_owner->SetDiffuse(glm::vec3(diffuse[0], diffuse[1], diffuse[2]));
		}
		//--------------------------------------------------------------------------+
		// Specular
		ImGui::Text("Directional Light : Specular");
		//glm::vec3 color;
		static float specular[3];
		specular[0] = m_owner->m_specular.x;
		specular[1] = m_owner->m_specular.y;
		specular[2] = m_owner->m_specular.z;

		label = "Directional Light Specular : R";
		ImGui::SliderFloat(label.c_str(), &specular[0], 0.0f, 1.0f);
		label = "Directional Light Specular : G";
		ImGui::SliderFloat(label.c_str(), &specular[1], 0.0f, 1.0f);
		label = "Directional Light Specular : B";
		ImGui::SliderFloat(label.c_str(), &specular[2], 0.0f, 1.0f);

		// カラーピッカーを使用するか
		ImGui::Checkbox("Color Picker", &m_colorPick);
		if (m_colorPick)
		{
			ImGui::ColorPicker3("Directional Light Color Picker", specular);
		}

		// 更新されたら
		if (specular[0] != m_owner->m_specular.x || specular[1] != m_owner->m_specular.y || specular[2] != m_owner->m_specular.z)
		{
			m_owner->SetSpecular(glm::vec3(specular[0], specular[1], specular[2]));
		}
		//--------------------------------------------------------------------------+
		// ambient
		ImGui::Text("Directional Light : Ambient");
		//glm::vec3 color;
		static float ambient[3];
		ambient[0] = m_owner->m_ambient.x;
		ambient[1] = m_owner->m_ambient.y;
		ambient[2] = m_owner->m_ambient.z;

		label = "Directional Light ambient : R";
		ImGui::SliderFloat(label.c_str(), &ambient[0], 0.0f, 1.0f);
		label = "Directional Light ambient : G";
		ImGui::SliderFloat(label.c_str(), &ambient[1], 0.0f, 1.0f);
		label = "Directional Light ambient : B";
		ImGui::SliderFloat(label.c_str(), &ambient[2], 0.0f, 1.0f);

		// カラーピッカーを使用するか
		ImGui::Checkbox("Color Picker", &m_colorPick);
		if (m_colorPick)
		{
			ImGui::ColorPicker3("Directional Light Color Picker", ambient);
		}

		// 更新されたら
		if (ambient[0] != m_owner->m_ambient.x || ambient[1] != m_owner->m_ambient.y || ambient[2] != m_owner->m_ambient.z)
		{
			m_owner->SetAmbient(glm::vec3(ambient[0], ambient[1], ambient[2]));
		}
		//--------------------------------------------------------------------------+

		ImGui::TreePop();
	}
}

void DirectionalLightDebug::Render()
{
}
