//----------------------------------------------------------------------------------+
// @file        ShaderManager.h
// @brief       �V�F�[�_�[�N���X�̊Ǘ�
//              �V�F�[�_�[�v���O��������ʂ��Ƃ�map�ɕۑ�
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/23   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include <string>
#include <iostream>
#include <unordered_map>
#include "GLSLprogram.h"

namespace GLSLshader
{
	enum SHADER_TYPE
	{
		SIMPLE_POS_COLOR,
		SIMPLE_POS_TEXTURE,
		BASIC_MESH,
		BASIC_MESH_TRIANGLE,
		BASIC_MESH_PHONG,
		GBUFFER_BASIC_MESH
	};
}

class ShaderManager
{

public:

	ShaderManager();
	~ShaderManager();

	bool CreateShaders();

	void EnableShaderProgram(GLSLshader::SHADER_TYPE _type);

	class GLSLprogram* GetShader(GLSLshader::SHADER_TYPE _type);

private:



	// �V�F�[�_�[�N���X�i�[�p�̘A�z�z��
	std::unordered_map<GLSLshader::SHADER_TYPE, class GLSLprogram*> m_shaders;



};