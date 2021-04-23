//----------------------------------------------------------------------------------+
// @file        BasicTriangle.h
// @brief       簡単な三角形オブジェクトの作成・描画オブジェクトとして登録
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/16   新規作成
// 2021/ 4/20   カラー情報追加
// 2021/ 4/23   テクスチャ座標追加
//----------------------------------------------------------------------------------+
#include "BasicTriangle.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "VertexArray.h"
#include "GLSLprogram.h"
#include "Texture.h"

BasicTriangle::BasicTriangle()
	:m_vao(0)
	,m_vbo(0)
	,m_diffuseMap(nullptr)
{

	// スケール(x, y, z)
	m_scale = glm::vec3(1.0);

	// 弧度
	m_radian = 1.0f;

	// ワールド変換行列
	m_worldTrans = glm::mat4(1.0);


	// カラー
	m_color = glm::vec3(0.0, 1.0, 0.0);

	// 頂点配列の定義 (座標、カラー、UV)
	float vertices[] =
	{
	//  |  x  |  y  |  z  |  u  |  v  |     R     |     G     |     B     
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, m_color.x, m_color.y, m_color.z,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, m_color.x, m_color.y, m_color.z,
		 0.0f,  0.5f, 0.0f, 0.5f, 1.0f, m_color.x, m_color.y, m_color.z
	};


	// 頂点配列・バッファをOpenGLに登録
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	// VAOにバインド
	glBindVertexArray(m_vao);

	// バッファに頂点情報を送る
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);


	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 頂点座標をアトリビュートにセット
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// テクスチャ座標をアトリビュートにセット
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// カラー情報の送信
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	// 各バインドを切る
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// テクスチャの生成
	m_diffuseMap = new Texture();
	m_diffuseMap->LoadTexture("Data/Textures/Test/TestTexture.jpg");

}

BasicTriangle::~BasicTriangle()
{
	delete m_diffuseMap;
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

/// <summary>
/// 描画処理
/// </summary>
void BasicTriangle::Draw(GLSLprogram* _shader)
{

	// 行列計算 (回転→拡大率)
	// m_worldTrans = glm::rotate(m_worldTrans, glm::radians(m_radian), glm::vec3(0.0, 0.0, 1.0));
	m_worldTrans = glm::scale(m_worldTrans, m_scale);

	// uniformへのセット
	_shader->SetUniform("u_worldTransform", m_worldTrans);

	// テクスチャのバインド
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_diffuseMap->GetTextureID());

	// 三角形の描画
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
