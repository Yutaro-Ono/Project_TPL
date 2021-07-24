//----------------------------------------------------------------------------------+
// @file        MeshComponent.h
// @brief       メッシュコンポーネントクラス
//              
// @note        Componentクラスを継承
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/14   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include "Component.h"
#include <string>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>


class MeshComponent : public Component
{

public:

	MeshComponent(class Actor* _owner);
	MeshComponent(class Actor* _owner, const std::string& _filePath);
	~MeshComponent();

	void Load(const std::string& _filePath);

	void DrawShadow(class GLSLprogram* _depthShader);
	void Draw(class GLSLprogram* _shader);

	void SetMaterialToUniform();


private:

	class Mesh* m_mesh;
	
	bool m_isVisible;       // 描画するか
	bool m_isDrawShadow;    // 影を描画するか
	bool m_isDrawHUD;       // HUDに描画するか
	
	glm::vec3 m_hudColor;   // HUDに描画する際のカラー

};