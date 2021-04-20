//----------------------------------------------------------------------------------+
// @file        BasicTriangle.h
// @brief       簡単な三角形オブジェクトの作成・描画オブジェクトとして登録
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/16   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class BasicTriangle
{

public:

	BasicTriangle();
	~BasicTriangle();

	void Draw();

	unsigned int GetTriangleVAO() { return m_vao; }     // 三角形用頂点配列オブジェクトのゲッター


private:

	// 頂点バッファ・配列用オブジェクト
	unsigned int m_vbo;
	unsigned int m_vao;

	// 行列関連
	glm::mat4 m_worldMat;
	glm::mat4 m_scaleMat;
	glm::mat4 m_rotationMat;

	class GLSLprogram* m_shader;       // シェーダー

};