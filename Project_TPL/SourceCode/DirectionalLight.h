//----------------------------------------------------------------------------------+
// @file        DirectionalLight.h
// @brief       ディレクショナルライトクラス
// @note        レンダラーに登録することで、描画時にライティングが反映される
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


	void SetDiffuse(const glm::vec3& _color) { m_diffuse = _color; }
	void SetSpecular(const glm::vec3& _color) { m_specular = _color; }
	void SetAmbient(const glm::vec3& _color) { m_ambient = _color; }
	void SetLightDirection(const glm::vec3& _lightDir) { m_lightDirection = _lightDir; }
	void SetIntensity(const float& _intens) { m_intensity = _intens; }

	const glm::vec3 GetDirection() const { return m_lightDirection; }
	const glm::vec3 GetDiffuse() const { return m_diffuse; }
	const glm::vec3 GetSpecular() const { return m_specular; }
	const glm::vec3 GetAmbient() const { return m_ambient; }
	const float& GetIntensity() const { return m_intensity; }

private:

	glm::vec3 m_diffuse;      // ディフューズ
	glm::vec3 m_specular;     // スペキュラ
	glm::vec3 m_ambient;           // アンビエント
	glm::vec3 m_lightDirection;    // 光の方向
	float m_intensity;             // 光の放出するエネルギー量

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