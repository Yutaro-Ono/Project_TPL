//----------------------------------------------------------------------------------+
// @file        GLSLprogram.h
// @brief       GLSL�V�F�[�_�[�̃R���p�C���E�����N���s��
// @note        �V�F�[�_�[�v���O�����̏���ێ�
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/24   �V�K�쐬
// 2021/ 3/25   �R���p�C���E�����N�����ǉ�
//----------------------------------------------------------------------------------+
#pragma once
#include <GL/glew.h>
#include <iostream>
#include <string>
#include "Math.h"
#include <sstream>
#include <fstream>


class GLSLprogram
{

public:

	GLSLprogram();
	~GLSLprogram();

	bool LoadShaders(const std::string&_vertPath, const std::string& _fragPath, const std::string& _geomPath);
	bool CompileShaderFromFile(const std::string& _shaderPath, GLenum _shaderType, GLuint& _outShader);
	bool LinkShaders(const unsigned int& _vertShader, const unsigned int& _fragShader, const unsigned int& _geomShader, unsigned int& _program);

	void Delete();

	bool IsCompiled(unsigned int _shader);

	bool IsLinkProgram();

	void UseProgram();

	// uniform�̃Z�b�g�֐�
	void SetUniform(const char* _name, int _val);
	void SetUniform(const char* _name, float _val);
	void SetUniform(const char* _name, bool _val);
	void SetUniform(const char* _name, const Vector2& _vec);
	void SetUniform(const char* _name, const Vector3& _vec);
	void SetUniform(const char* _name, const Matrix3& _mat);
	void SetUniform(const char* _name, const Matrix4& _mat);


private:


	unsigned int m_vertexShader;
	unsigned int m_fragmentShader;
	unsigned int m_geometryShader;
	unsigned int m_shaderProgram;         // �V�F�[�_�[�v���O����

};