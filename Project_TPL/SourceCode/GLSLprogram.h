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
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <string>

namespace GLSLshader
{
	enum SHADER_TYPE
	{
		BASIC_MESH,
		GBUFFER_MESH
	};
}

class GLSLprogram
{

public:

	GLSLprogram(const std::string& _vertPath, const std::string& _fragPath, const std::string& _geomPath = nullptr);
	~GLSLprogram();

	bool LoadShaders(const std::string& _vertPath, const std::string& _fragPath, const std::string& _geomPath);
	bool CompileShaderFromFile(const std::string& _shaderPath, GLenum _shaderType, GLuint& _outShader);
	bool LinkShaders(const GLuint& _vertShader, const GLuint& _fragShader, const GLuint& _geomShader, GLuint& _program);

	void SetUniformBlockFromIndex(const std::string& _uniformIndex);

	void Delete();

	bool IsCompiled(GLuint _shader);

	bool IsLinkProgram();

	void UseProgram();

	// uniformのセット関数
	void SetUniform(const char* _name, int _val);
	void SetUniform(const char* _name, float _val);
	void SetUniform(const char* _name, bool _val);
	void SetUniform(const char* _name, const glm::vec2& _vec);
	void SetUniform(const char* _name, const glm::vec3& _vec);
	void SetUniform(const char* _name, const glm::vec4& _vec);
	void SetUniform(const char* _name, const glm::mat3& _mat);
	void SetUniform(const char* _name, const glm::mat4& _mat);


private:

	GLuint m_shaderProgram;         // シェーダープログラム

	GLuint m_uniformBlockIndex;     // ユニフォームバッファID
	GLuint m_uniformBlockBinding;   // ユニフォームブロックのバインド番号

};