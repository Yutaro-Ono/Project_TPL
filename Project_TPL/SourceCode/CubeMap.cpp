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
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include "VertexArray.h"
#include "GLSLprogram.h"

CubeMap::CubeMap()
	:m_cubeVerts(nullptr)
{
	m_cubeVerts = new VertexArray();
	m_cubeVerts->CreateCubeMapVertices();
}

CubeMap::~CubeMap()
{
	delete m_cubeVerts;
}

/// <summary>
/// キューブマップ用テクスチャのロード
/// ※6面
/// </summary>
/// <param name="_filePath"></param>
/// <returns></returns>
bool CubeMap::LoadCubeMapTextures(const std::string& _filePath)
{
	// キューブマップとして登録
	glGenTextures(1, &m_cubeMapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapID);

	// 各面のテクスチャパス
	std::string faces[] =
	{
		_filePath + "_right.jpg",
		_filePath + "_left.jpg",
		_filePath + "_top.jpg",
		_filePath + "_bottom.jpg",
		_filePath + "_front.jpg",
		_filePath + "_back.jpg"
	};

	int width, height, channels;

	for (unsigned int i = 0; i < 6; ++i)
	{
		
		// 1面のテクスチャをロード
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);

		if (data != NULL)
		{
			// 浮動小数として保存
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
				         width, height, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		else
		{
			std::cout << "Failed::Load CubeMap Face Texture::" << faces[i] << std::endl;
			return false;
		}

		// 解放
		stbi_image_free(data);

	}

	// テクスチャパラメータ設定
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// バインド解除
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}

/// <summary>
/// キューブマップ描画処理
/// </summary>
/// <param name="_shader"> キューブマップシェーダー </param>
void CubeMap::Draw(GLSLprogram* _shader)
{

	// 深度設定
	glDepthFunc(GL_LEQUAL);

	// キューブマップ用テクスチャのバインド
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapID);
	// キューブマップの頂点配列をアクティブにする
	m_cubeVerts->SetActive();
	// 描画
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDepthFunc(GL_LESS);
}
