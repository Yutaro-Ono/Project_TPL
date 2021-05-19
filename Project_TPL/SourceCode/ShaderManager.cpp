//----------------------------------------------------------------------------------+
// @file        ShaderManager.cpp
// @brief       シェーダークラスの管理
//              シェーダープログラムを種別ごとにmapに保存
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/23   新規作成
//----------------------------------------------------------------------------------+
#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
	// シェーダー連想配列の削除
	auto iter = m_shaders.begin();
	while (iter != m_shaders.end())
	{
		m_shaders.erase(iter++);
	}
}

/// <summary>
/// シェーダークラスの生成
/// </summary>
/// <returns> シェーダープログラムの作成に失敗した場合にfalseを返す </returns>
bool ShaderManager::CreateShaders()
{
	m_shaders[GLSLshader::SIMPLE_POS_COLOR] = new GLSLprogram();
	if (!m_shaders[GLSLshader::SIMPLE_POS_COLOR]->LoadShaders("Shaders/SimpleShader_pos_color.vert", "Shaders/SimpleShader_pos_color.frag"))
	{
		return false;
	}

	m_shaders[GLSLshader::SIMPLE_POS_TEXTURE] = new GLSLprogram();
	if (!m_shaders[GLSLshader::SIMPLE_POS_TEXTURE]->LoadShaders("Shaders/BasicShader.vert", "Shaders/BasicShader_SamplingTexture.frag"))
	{
		return false;
	}
	m_shaders[GLSLshader::SIMPLE_POS_TEXTURE]->SetUniform("u_mat.diffuseMap", 0);


	m_shaders[GLSLshader::BASIC_MESH] = new GLSLprogram();
	if (!m_shaders[GLSLshader::BASIC_MESH]->LoadShaders("Shaders/BasicMesh.vert", "Shaders/BasicMesh.frag"))
	{
		return false;
	}
	m_shaders[GLSLshader::BASIC_MESH]->SetUniform("u_mat.albedo", 0);

	return true;
}


/// <summary>
/// 指定したタイプのシェーダープログラムの有効化
/// </summary>
/// <param name=GLSLshader::SHADER_TYPE> シェーダーのタイプ </param>
void ShaderManager::EnableShaderProgram(GLSLshader::SHADER_TYPE _type)
{
	m_shaders[_type]->UseProgram();
}

/// <summary>
/// シェーダークラスのポインタのゲッター
/// </summary>
/// <param name="_type"> シェーダーのタイプ </param>
/// <returns> 指定したタイプのシェーダークラスポインタを返す </returns>
GLSLprogram* ShaderManager::GetShader(GLSLshader::SHADER_TYPE _type)
{
	return m_shaders[_type];
}
