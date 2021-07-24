//----------------------------------------------------------------------------------+
// @file        GLSLprogram.cpp
// @brief       GLSL�V�F�[�_�[�̃R���p�C���E�����N���s��
// @note        �V�F�[�_�[�v���O�����̏���ێ�
// @author      ���� �N���Y (Yutaro Ono, @2021)
//----------------------------------------------------------------------------------+
#include "GLSLprogram.h"


GLSLprogram::GLSLprogram()
    :m_shaderProgram(0)
{
}

GLSLprogram::~GLSLprogram()
{
    Delete();
}


/// <summary>
/// �V�F�[�_�[�̓ǂݍ��݁E�R���p�C���E�����N����
/// </summary>
/// <param name="_vertPath"> ���_�V�F�[�_�[�̃t�@�C���p�X </param>
/// <param name="_fragPath"> �t���O�����g�V�F�[�_�[�̃t�@�C���p�X </param>
/// <param name="_geomPath"> �W�I���g���V�F�[�_�[�̃t�@�C���p�X </param>
/// <returns> ���[�h�����ɐ���������(true)�E���s������(false) </returns>
bool GLSLprogram::LoadShaders(const char* _vertPath, const char* _fragPath, const char* _geomPath)
{

    // �t�@�C���p�X����V�F�[�_�[�\�[�X�R�[�h���擾����
    std::string vertCode;
    std::string fragCode;
    std::string geomCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;

    // ifstream�I�u�W�F�N�g����O���X���[�ł��邩�m�F
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);


    try
    {
        // �V�F�[�_�[�t�@�C�����J��
        vShaderFile.open(_vertPath);
        fShaderFile.open(_fragPath);

        // �t�@�C�����̃o�b�t�@��ǂݎ��Astream�Ɋi�[
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // �t�@�C���̃n���h�������
        vShaderFile.close();
        fShaderFile.close();

        // stream�̓��e��string�ɕϊ�
        vertCode = vShaderStream.str();
        fragCode = fShaderStream.str();

        // �W�I���g���V�F�[�_�[���w�肳��Ă����ꍇ
        if (_geomPath != nullptr)
        {
            gShaderFile.open(_geomPath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geomCode = gShaderStream.str();
        }

    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "Error::Shader::File Read Failed" << std::endl;
        return false;
    }

    // ������ɕۊ�
    const char* vShaderCode = vertCode.c_str();
    const char* fShaderCode = fragCode.c_str();

    // �V�F�[�_�[�̃R���p�C��
    unsigned int vertex, fragment;

    // ���_�V�F�[�_�[
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    
    // �t���O�����g�V�F�[�_�[
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // ����ɃR���p�C���ł������`�F�b�N
    if (!CompileShaderFromFile(_vertPath, GL_VERTEX_SHADER, vertex) ||
        !CompileShaderFromFile(_fragPath, GL_FRAGMENT_SHADER, fragment))
    {
        return false;
    }

    // �W�I���g���V�F�[�_�[
    unsigned int geometry = 0;
    if (_geomPath != nullptr)
    {

        const char* gShaderCode = geomCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);

        // ����ɃR���p�C���ł������`�F�b�N
        if (!CompileShaderFromFile(_geomPath, GL_GEOMETRY_SHADER, geometry))
        {
            return false;
        }
    }

    // �V�F�[�_�[�v���O�����ւ̃����N
    if (!LinkShaders(vertex, fragment, geometry, m_shaderProgram))
    {
        return false;
    }

    // uniform�u���b�N�ւ̃����N
    // �s�� (�r���[�E�v���W�F�N�V����)
    unsigned int uniformBlockIndex = glGetUniformBlockIndex(m_shaderProgram, "Matrices");
    glUniformBlockBinding(m_shaderProgram, uniformBlockIndex, 0);
    // �J�����֌W (���W�A����)
    uniformBlockIndex = glGetUniformBlockIndex(m_shaderProgram, "CameraVariable");
    glUniformBlockBinding(m_shaderProgram, uniformBlockIndex, 1);
    // �J�����֌W (���W�A����)
    uniformBlockIndex = glGetUniformBlockIndex(m_shaderProgram, "Triggers");
    glUniformBlockBinding(m_shaderProgram, uniformBlockIndex, 2);
    // �J�����֌W (���W�A����)
    uniformBlockIndex = glGetUniformBlockIndex(m_shaderProgram, "DirLight");
    glUniformBlockBinding(m_shaderProgram, uniformBlockIndex, 3);

    return true;
}

/// <summary>
/// �������
/// </summary>
void GLSLprogram::Delete()
{
    // �V�F�[�_�[�v���O�����̉��
    glDeleteProgram(m_shaderProgram);
}

/// <summary>
/// �V�F�[�_�[�t�@�C����ǂݍ��݁A�R���p�C������
/// </summary>
/// <param name="_shaderPath"> �R���p�C������V�F�[�_�[�t�@�C�� </param>
/// <param name="_shaderType"> �V�F�[�_�[�̎�� (���_�A�t���O�����getc...) </param>
/// <param name="_outShader"> �R���p�C�������V�F�[�_�[���i�[����ϐ� </param>
/// <returns> �R���p�C������������true��Ԃ� </returns>
bool GLSLprogram::CompileShaderFromFile(const char* _shaderPath, GLenum _shaderType, unsigned int& _outShader)
{


    // �V�F�[�_�[�t�@�C�����I�[�v��
    std::ifstream shaderFile(_shaderPath);

    if (shaderFile.is_open())
    {
        // �ǂݍ��񂾃V�F�[�_�[�t�@�C���̕������ϊ�
        std::stringstream transText;
        transText << shaderFile.rdbuf();

        std::string contents = transText.str();
        
        const char* contentsChar = contents.c_str();

        // �����ɉ������^�C�v�̃V�F�[�_�[ID���쐬�E�ۊ�
        _outShader = glCreateShader(_shaderType);

        // ��ŕϊ������������p���ăV�F�[�_�[���R���p�C������
        glShaderSource(_outShader, 1, &(contentsChar), nullptr);
        glCompileShader(_outShader);

        // ����ɃR���p�C���ł������`�F�b�N
        if (!IsCompiled(_outShader))
        {
            std::cout << "Failed::Shader::Compiled " << _shaderPath << std::endl;
            return false;
        }

    }
    else
    {
        std::cout << "Error::Shader::" << _shaderPath << " Not Found" << std::endl;

        return false;
    }


    return true;
}


/// <summary>
/// �V�F�[�_�[�v���O�����ւ̃����N���s���֐�
/// </summary>
/// <returns> �����N���ꂽ��true��Ԃ� </returns>
bool GLSLprogram::LinkShaders(const unsigned int& _vertShader, const unsigned int& _fragShader, const unsigned int& _geomShader, unsigned int& _program)
{
    // �V�F�[�_�[�v���O�����̍쐬
    m_shaderProgram = glCreateProgram();

    // �V�F�[�_�[�v���O�����ɒ��_�V�F�[�_�[�A�t���O�����g�V�F�[�_�[���A�^�b�`
    // ���V�F�[�_�[�����N�ɕK�v�Ȏ��O����
    glAttachShader(m_shaderProgram, _vertShader);
    glAttachShader(m_shaderProgram, _fragShader);

    if (_geomShader != 0)
    {
        glAttachShader(m_shaderProgram, _geomShader);
    }


    // �V�F�[�_�[�v���O�����������N
    glLinkProgram(_program);

    // ����Ƀ����N�ł������`�F�b�N
    if (!IsLinkProgram())
    {
        std::cout << "Failed::Shader::Link" << std::endl;

        return false;
    }


    return true;
}


/// <summary>
/// �V�F�[�_�[������ɃR���p�C�����ꂽ�������؂���
/// </summary>
/// <param name="_shader"> ���؂������V�F�[�_�[ </param>
/// <returns> ����ɃR���p�C������Ă����ꍇ��true��Ԃ� </returns>
bool GLSLprogram::IsCompiled(unsigned int _shader)
{
    GLint status;

    // �R���p�C���̏�Ԃ�₢���킹��
    glGetShaderiv(_shader, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE)
    {
        char buffer[512];
        memset(buffer, 0, 512);

        glGetShaderInfoLog(_shader, 511, nullptr, buffer);

        std::cout << buffer << std::endl;

        return false;
    }

    return true;
}


/// <summary>
/// �V�F�[�_�[�v���O�����������N���ꂽ�������؂���
/// </summary>
/// <returns> �ُ킪�Ȃ����true��Ԃ� </returns>
bool GLSLprogram::IsLinkProgram()
{
    int status;

    // �V�F�[�_�[�v���O�����̃����N��Ԃ�₢���킹��
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &status);

    if (status != GL_TRUE)
    {
        char buffer[512];
        memset(buffer, 0, 512);

        glGetProgramInfoLog(m_shaderProgram, 511, nullptr, buffer);
        
        std::cout << buffer << std::endl;

        return false;
    }


    return true;
}

/// <summary>
/// �V�F�[�_�[�v���O�����݂̂��g�p����
/// </summary>
void GLSLprogram::UseProgram()
{
    glUseProgram(m_shaderProgram);
}

//--------------------------------------------------------------+
// uniform�Z�b�g�֐��Q
//--------------------------------------------------------------+
void GLSLprogram::SetUniform(const char* _name, int _val)
{
    int loc = glGetUniformLocation(m_shaderProgram, _name);

    if (loc >= 0)
    {
        glUniform1i(loc, _val);
    }
}

void GLSLprogram::SetUniform(const char* _name, float _val)
{
    int loc = glGetUniformLocation(m_shaderProgram, _name);

    if (loc >= 0)
    {
        glUniform1f(loc, _val);
    }
}

void GLSLprogram::SetUniform(const char* _name, bool _val)
{
    int loc = glGetUniformLocation(m_shaderProgram, _name);

    if (loc >= 0)
    {
        glUniform1i(loc, _val);
    }
}

void GLSLprogram::SetUniform(const char* _name, const Vector2& _vec)
{
    int loc = glGetUniformLocation(m_shaderProgram, _name);

    if (loc >= 0)
    {
        glUniform2f(loc, _vec.x, _vec.y);
    }
}

void GLSLprogram::SetUniform(const char* _name, const Vector3& _vec)
{
    int loc = glGetUniformLocation(m_shaderProgram, _name);

    if (loc >= 0)
    {
        glUniform3f(loc, _vec.x, _vec.y, _vec.z);
    }
}


void GLSLprogram::SetUniform(const char* _name, const Matrix3& _mat)
{
    int loc = glGetUniformLocation(m_shaderProgram, _name);

    if (loc >= 0)
    {
        glUniformMatrix3fv(loc, 1, GL_FALSE, _mat.GetAsFloatPtr());
    }
}

void GLSLprogram::SetUniform(const char* _name, const Matrix4& _mat)
{
    int loc = glGetUniformLocation(m_shaderProgram, _name);

    if (loc >= 0)
    {
        glUniformMatrix4fv(loc, 1, GL_FALSE, _mat.GetAsFloatPtr());
    }
}
