//----------------------------------------------------------------------------------+
// @file        MeshComponent.h
// @brief       ���b�V���R���|�[�l���g�N���X
//              
// @note        Component�N���X���p��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/14   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "Component.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class MeshComponent : public Component
{

public:

	MeshComponent(class Actor* _owner);
	MeshComponent(class Actor* _owner, const std::string& _filePath);
	~MeshComponent();

	void Load(const std::string& _filePath);

	void DrawShadow(class GLSLprogram* _depthShader);
	void Draw(class GLSLprogram* _shader);




private:

	class Mesh* m_mesh;
	
	bool m_isVisible;       // �`�悷�邩
	bool m_isDrawShadow;    // �e��`�悷�邩
	bool m_isDrawHUD;       // HUD�ɕ`�悷�邩
	
	glm::vec3 m_hudColor;   // HUD�ɕ`�悷��ۂ̃J���[

};