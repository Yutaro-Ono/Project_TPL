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
#include "Math.h"
#include "DebugObjectBase.h"

class DirectionalLight
{

public:

	DirectionalLight();
	~DirectionalLight();


	void SetDiffuse(const Vector3& _color) { m_diffuse = _color; }
	void SetSpecular(const Vector3& _color) { m_specular = _color; }
	void SetAmbient(const Vector3& _color) { m_ambient = _color; }
	void SetLightDirection(const Vector3& _lightDir) { m_lightDirection = _lightDir; }
	void SetIntensity(const float& _intens) { m_intensity = _intens; }

	const Vector3& GetPosition() const { return m_position; }
	const Vector3& GetDirection() const { return m_lightDirection; }
	const Vector3& GetDiffuse() const { return m_diffuse; }
	const Vector3& GetSpecular() const { return m_specular; }
	const Vector3& GetAmbient() const { return m_ambient; }
	const float& GetIntensity() const { return m_intensity; }

private:

	Vector3 m_diffuse;      // ディフューズ
	Vector3 m_specular;     // スペキュラ
	Vector3 m_ambient;           // アンビエント
	Vector3 m_position;
	Vector3 m_lightDirection;    // 光の方向
	float m_intensity;             // 光の放出するエネルギー量

	class DirectionalLightDebug* m_debugObj;
	friend class DirectionalLightDebug;
	friend class Renderer;

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

	bool m_diffuseColorPick;
	bool m_specularColorPick;
	bool m_ambientColorPick;

};