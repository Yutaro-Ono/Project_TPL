//----------------------------------------------------------------------------------+
// @file        BasicTriangle.h
// @brief       簡単な三角形オブジェクトの作成・描画オブジェクトとして登録
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/16   新規作成
//----------------------------------------------------------------------------------+
#include "BasicTriangle.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "VertexArray.h"
#include "GLSLprogram.h"

BasicTriangle::BasicTriangle()
	:m_vao(0)
	,m_vbo(0)
	,m_shader(nullptr)
{

	// ワールド変換行列
	m_worldMat = glm::mat4(1.0);

	// シェーダークラスの生成
	m_shader = new GLSLprogram("Shaders/BasicShader.vert", "Shaders/BasicShader.frag");

	// 簡単な三角形の頂点配列
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	// 頂点配列・バッファをOpenGLに登録
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	// VAOにバインド
	glBindVertexArray(m_vao);

	// バッファに頂点情報を送る
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 頂点配列の情報をアトリビュートにセット
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 各バインドを切る
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

BasicTriangle::~BasicTriangle()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
	delete m_shader;
}

/// <summary>
/// 描画処理
/// </summary>
void BasicTriangle::Draw()
{
	m_shader->UseProgram();

	// ユニフォーム
	m_shader->SetUniform("u_worldMat", m_worldMat);
	

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
