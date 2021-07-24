//----------------------------------------------------------------------------------+
// @file        CubeMap.cpp
// @brief       キューブマップの定義・情報の保持
//              
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 6/22   新規作成
//----------------------------------------------------------------------------------+
#include "CubeMap.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "VertexArray.h"
#include "GLSLprogram.h"
#include "Texture.h"

CubeMap::CubeMap(const std::string& _filePath)
	:m_cubeVerts(nullptr)
{
	m_cubeVerts = new VertexArray();
	m_cubeVerts->CreateCubeMapVertices();

	Texture* tex = new Texture;
	m_cubeMapID = tex->LoadCubeMapTextures(_filePath);

}

CubeMap::~CubeMap()
{
	delete m_cubeVerts;
}


/// <summary>
/// キューブマップ描画処理
/// </summary>
/// <param name="_shader"> キューブマップシェーダー </param>
void CubeMap::Draw(GLSLprogram* _shader)
{

	// 深度設定
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);

	// キューブマップ用テクスチャのバインド
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapID);
	// キューブマップの頂点配列をアクティブにする
	m_cubeVerts->SetActive();
	// 描画
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}
