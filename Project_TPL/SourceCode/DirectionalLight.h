//----------------------------------------------------------------------------------+
// @file        DirectionalLight.h
// @brief       �f�B���N�V���i�����C�g�N���X
// @note        �����_���[�ɓo�^���邱�ƂŁA�`�掞�Ƀ��C�e�B���O�����f�����
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

	glm::vec3 m_diffuse;      // �f�B�t���[�Y
	glm::vec3 m_specular;     // �X�y�L����
	glm::vec3 m_ambient;           // �A���r�G���g
	glm::vec3 m_lightDirection;    // ���̕���
	float m_intensity;             // ���̕��o����G�l���M�[��

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