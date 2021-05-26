//----------------------------------------------------------------------------------+
// @file        DirectionalLight.h
// @brief       ディレクショナルライトクラス
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/26   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include <glm/glm.hpp>
#include "DebugObjectBase.h"

class DirectionalLight
{

public:

	DirectionalLight();
	~DirectionalLight();


	void SetLightColor(const glm::vec3& _color) { m_lightColor = _color; }
	void SetLightDirection(const glm::vec3& _lightDir) { m_lightDirection = _lightDir; }
	void SetIntensity(const float& _intens) { m_intensity = _intens; }

private:

	glm::vec3 m_lightColor;      // ライトが放つ色
	glm::vec3 m_lightDirection;  // 光の方向
	float m_intensity;           // 光の放出するエネルギー量

	class DirectionalLightDebug* m_debugObj;
	friend class DirectionalLightDebug;

};



//----------------------------------------------------------------------------------+
// @file        DirectionalLightDebug.h
// @brief       ディレクショナルライトのデバッグクラス
// @note        DebugObjectBaseクラスを継承
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/26   新規作成
//----------------------------------------------------------------------------------+
class DirectionalLightDebug : public DebugObjectBase
{

public:

	DirectionalLightDebug(class DirectionalLight* _owner);
	~DirectionalLightDebug();

	void Update(float _deltaTime) override;
	void Render() override;

private:


	class DirectionalLight* m_owner;

	bool m_colorPick;

};