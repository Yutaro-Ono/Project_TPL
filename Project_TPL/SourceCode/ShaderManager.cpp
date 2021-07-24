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
	if (!m_shaders[GLSLshader::SIMPLE_POS_COLOR]->LoadShaders("Shaders/SimpleShader_pos_color.vert", "Shaders/SimpleShader_pos_color.frag", ""))
	{
		return false;
	}

	m_shaders[GLSLshader::SIMPLE_POS_TEXTURE] = new GLSLprogram();
	if (!m_shaders[GLSLshader::SIMPLE_POS_TEXTURE]->LoadShaders("Shaders/BasicShader.vert", "Shaders/BasicShader_SamplingTexture.frag", ""))
	{
		return false;
	}
	m_shaders[GLSLshader::SIMPLE_POS_TEXTURE]->SetUniform("u_mat.diffuseMap", 0);


	m_shaders[GLSLshader::BASIC_MESH] = new GLSLprogram();
	if (!m_shaders[GLSLshader::BASIC_MESH]->LoadShaders("Shaders/BasicMesh.vert", "Shaders/BasicMesh.frag", ""))
	{
		return false;
	}
	m_shaders[GLSLshader::BASIC_MESH]->SetUniform("u_mat.albedo", 0);

	m_shaders[GLSLshader::GBUFFER_BASIC_MESH] = new GLSLprogram();
	if (!m_shaders[GLSLshader::GBUFFER_BASIC_MESH]->LoadShaders("Shaders/GBuffer_Basic.vert", "Shaders/GBuffer_Basic.frag", ""))
	{
		return false;
	}

	m_shaders[GLSLshader::GBUFFER_PHONG] = new GLSLprogram();
	if (!m_shaders[GLSLshader::GBUFFER_PHONG]->LoadShaders("Shaders/GBuffer_Phong.vert", "Shaders/GBuffer_Phong.frag", ""))
	{
		return false;
	}

	m_shaders[GLSLshader::GBUFFER_NORMALMAP] = new GLSLprogram();
	if (!m_shaders[GLSLshader::GBUFFER_NORMALMAP]->LoadShaders("Shaders/GBuffer_NormalMap.vert", "Shaders/GBuffer_NormalMap.frag", ""))
	{
		return false;
	}

	m_shaders[GLSLshader::GBUFFER_BASIC_SKYBOX] = new GLSLprogram();
	if (!m_shaders[GLSLshader::GBUFFER_BASIC_SKYBOX]->LoadShaders("Shaders/GBuffer_Basic_SkyBox.vert", "Shaders/GBuffer_Basic_SkyBox.frag", ""))
	{
		return false;
	}
	m_shaders[GLSLshader::GBUFFER_BASIC_SKYBOX]->SetUniform("u_cubeMap", 0);

	m_shaders[GLSLshader::OUT_SCREEN_ENTIRE] = new GLSLprogram();
	if (!m_shaders[GLSLshader::OUT_SCREEN_ENTIRE]->LoadShaders("Shaders/OutScreen.vert", "Shaders/OutScreen.frag", ""))
	{
		return false;
	}
	m_shaders[GLSLshader::OUT_SCREEN_ENTIRE]->SetUniform("u_screenTexture", 0);


	//-------------------------------------------------------------------------+
	// Bloom用シェーダー
	//-------------------------------------------------------------------------+
	m_shaders[GLSLshader::BLOOM_DOWNSAMPLING] = new GLSLprogram();
	if (!m_shaders[GLSLshader::BLOOM_DOWNSAMPLING]->LoadShaders("Shaders/OutScreen.vert", "Shaders/Bloom_DownSampling.frag", ""))
	{
		return false;
	}
	m_shaders[GLSLshader::BLOOM_DOWNSAMPLING]->SetUniform("u_scene", 0);

	m_shaders[GLSLshader::BLOOM_GAUSSIAN_BLUR] = new GLSLprogram();
	if (!m_shaders[GLSLshader::BLOOM_GAUSSIAN_BLUR]->LoadShaders("Shaders/OutScreen.vert", "Shaders/Bloom_GaussianBlur.frag", ""))
	{
		return false;
	}
	m_shaders[GLSLshader::BLOOM_GAUSSIAN_BLUR]->SetUniform("u_blurSource", 0);

	m_shaders[GLSLshader::BLOOM_TONEMAPPING] = new GLSLprogram();
	if (!m_shaders[GLSLshader::BLOOM_TONEMAPPING]->LoadShaders("Shaders/OutScreen.vert", "Shaders/Bloom_ToneMapping.frag", ""))
	{
		return false;
	}
	m_shaders[GLSLshader::BLOOM_TONEMAPPING]->SetUniform("u_scene", 0);

	//---------------------------------------------------------------------------+
	// ライトパス
	//---------------------------------------------------------------------------+
	m_shaders[GLSLshader::DIRECTIONAL_LIGHT_PASS] = new GLSLprogram();
	if (!m_shaders[GLSLshader::DIRECTIONAL_LIGHT_PASS]->LoadShaders("Shaders/GBuffer_LightPass.vert", "Shaders/GBuffer_DirectionalLight.frag", ""))
	{
		return false;
	}

	//---------------------------------------------------------------------------+
	// その他のシェーダー (デバッグ用ビジュアライザー系)
	//---------------------------------------------------------------------------+
	m_shaders[GLSLshader::OPTION_NORMAL_VISUALIZE] = new GLSLprogram();
	if (!m_shaders[GLSLshader::OPTION_NORMAL_VISUALIZE]->LoadShaders("Shaders/option/Normal_Visualization.vert", "Shaders/option/Normal_Visualization.frag", "Shaders/option/Normal_Visualization.geom"))
	{
		return false;
	}

	m_shaders[GLSLshader::OPTION_NORMAL_VISUALIZE_GBUFFER] = new GLSLprogram();
	if (!m_shaders[GLSLshader::OPTION_NORMAL_VISUALIZE_GBUFFER]->LoadShaders("Shaders/option/Normal_Visualization_GBuffer.vert", "Shaders/option/Normal_Visualization_GBuffer.frag", "Shaders/option/Normal_Visualization_GBuffer.geom"))
	{
		return false;
	}

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
