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

	// uniform�̃Z�b�g�֐�
	void SetUniform(const char* _name, int _val);
	void SetUniform(const char* _name, float _val);
	void SetUniform(const char* _name, bool _val);
	void SetUniform(const char* _name, const glm::vec2& _vec);
	void SetUniform(const char* _name, const glm::vec3& _vec);
	void SetUniform(const char* _name, const glm::vec4& _vec);
	void SetUniform(const char* _name, const glm::mat3& _mat);
	void SetUniform(const char* _name, const glm::mat4& _mat);


private:

	GLuint m_shaderProgram;         // �V�F�[�_�[�v���O����

	GLuint m_uniformBlockIndex;     // ���j�t�H�[���o�b�t�@ID
	GLuint m_uniformBlockBinding;   // ���j�t�H�[���u���b�N�̃o�C���h�ԍ�

};