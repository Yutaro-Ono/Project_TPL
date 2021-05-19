//----------------------------------------------------------------------------------+
// @file        ShaderManager.cpp
// @brief       �V�F�[�_�[�N���X�̊Ǘ�
//              �V�F�[�_�[�v���O��������ʂ��Ƃ�map�ɕۑ�
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/23   �V�K�쐬
//----------------------------------------------------------------------------------+
#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
	// �V�F�[�_�[�A�z�z��̍폜
	auto iter = m_shaders.begin();
	while (iter != m_shaders.end())
	{
		m_shaders.erase(iter++);
	}
}

/// <summary>
/// �V�F�[�_�[�N���X�̐���
/// </summary>
/// <returns> �V�F�[�_�[�v���O�����̍쐬�Ɏ��s�����ꍇ��false��Ԃ� </returns>
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
/// �w�肵���^�C�v�̃V�F�[�_�[�v���O�����̗L����
/// </summary>
/// <param name=GLSLshader::SHADER_TYPE> �V�F�[�_�[�̃^�C�v </param>
void ShaderManager::EnableShaderProgram(GLSLshader::SHADER_TYPE _type)
{
	m_shaders[_type]->UseProgram();
}

/// <summary>
/// �V�F�[�_�[�N���X�̃|�C���^�̃Q�b�^�[
/// </summary>
/// <param name="_type"> �V�F�[�_�[�̃^�C�v </param>
/// <returns> �w�肵���^�C�v�̃V�F�[�_�[�N���X�|�C���^��Ԃ� </returns>
GLSLprogram* ShaderManager::GetShader(GLSLshader::SHADER_TYPE _type)
{
	return m_shaders[_type];
}
