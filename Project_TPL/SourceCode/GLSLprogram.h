//----------------------------------------------------------------------------------+
// @file        GLSLprogram.h
// @brief       GLSLシェーダーのコンパイル・リンクを行う
// @note        シェーダープログラムの情報を保持
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/24   新規作成
// 2021/ 3/25   コンパイル・リンク処理追加
//----------------------------------------------------------------------------------+
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include "Math.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>


class GLSLprogram
{

public:

	GLSLprogram();
	~GLSLprogram();

	bool LoadShaders(const char* _vertPath, const char* _fragPath, const char* _geomPath = nullptr);
	bool CompileShaderFromFile(const char* _shaderPath, GLenum _shaderType, unsigned int& _outShader);
	bool LinkShaders(const unsigned int& _vertShader, const unsigned int& _fragShader, const unsigned int& _geomShader, unsigned int& _program);

	void Delete();

	bool IsCompiled(unsigned int _shader);

	bool IsLinkProgram();

	void UseProgram();

	// uniformのセット関数
	void SetUniform(const char* _name, int _val);
	void SetUniform(const char* _name, float _val);
	void SetUniform(const char* _name, bool _val);
	void SetUniform(const char* _name, const Vector2& _vec);
	void SetUniform(const char* _name, const Vector3& _vec);
	void SetUniform(const char* _name, const Matrix3& _mat);
	void SetUniform(const char* _name, const Matrix4& _mat);


private:

	GLuint m_shaderProgram;         // シェーダープログラム

};