//----------------------------------------------------------------------------------+
// @file        DirectionalLight.h
// @brief       �f�B���N�V���i�����C�g�N���X
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/26   �V�K�쐬
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

	glm::vec3 m_lightColor;      // ���C�g�����F
	glm::vec3 m_lightDirection;  // ���̕���
	float m_intensity;           // ���̕��o����G�l���M�[��

	class DirectionalLightDebug* m_debugObj;
	friend class DirectionalLightDebug;

};



//----------------------------------------------------------------------------------+
// @file        DirectionalLightDebug.h
// @brief       �f�B���N�V���i�����C�g�̃f�o�b�O�N���X
// @note        DebugObjectBase�N���X���p��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/26   �V�K�쐬
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