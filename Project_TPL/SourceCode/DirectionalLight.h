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

	Vector3 m_diffuse;      // �f�B�t���[�Y
	Vector3 m_specular;     // �X�y�L����
	Vector3 m_ambient;           // �A���r�G���g
	Vector3 m_position;
	Vector3 m_lightDirection;    // ���̕���
	float m_intensity;             // ���̕��o����G�l���M�[��

	class DirectionalLightDebug* m_debugObj;
	friend class DirectionalLightDebug;
	friend class Renderer;

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

	bool m_diffuseColorPick;
	bool m_specularColorPick;
	bool m_ambientColorPick;

};