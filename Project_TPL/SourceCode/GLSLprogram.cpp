//----------------------------------------------------------------------------------+
// @file        GLSLprogram.cpp
// @brief       GLSLシェーダーのコンパイル・リンクを行う
// @note        シェーダープログラムの情報を保持
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//----------------------------------------------------------------------------------+
#include "GLSLprogram.h"

GLSLprogram::GLSLprogram()
    :m_vertexShader(0)
    ,m_fragmentShader(0)
    ,m_geometryShader(0)
    ,m_shaderProgram(0)
{
}

GLSLprogram::~GLSLprogram()
{
    Delete();
}


/// <summary>
/// シェーダーの読み込み・コンパイル・リンク処理
/// </summary>
/// <param name="_vertPath"> 頂点シェーダーのファイルパス </param>
/// <param name="_fragPath"> フラグメントシェーダーのファイルパス </param>
/// <param name="_geomPath"> ジオメトリシェーダーのファイルパス </param>
/// <returns> ロード処理に成功したか(true)・失敗したか(false) </returns>
bool GLSLprogram::LoadShaders(const std::string& _vertPath, const std::string& _fragPath, const std::string& _geomPath)
{

    // シェーダーのコンパイル
    if (!CompileShaderFromFile(_vertPath, GL_VERTEX_SHADER, m_vertexShader) ||
        !CompileShaderFromFile(_fragPath, GL_FRAGMENT_SHADER, m_fragmentShader))
    {
        return false;
    }

    // ジオメトリシェーダー
    if (_geomPath.size() != 0)
    {
        // シェーダーのコンパイル
        if (!CompileShaderFromFile(_geomPath, GL_GEOMETRY_SHADER, m_geometryShader))
        {
            return false;
        }
    }

    // シェーダープログラムへのリンク
    if (!LinkShaders(m_vertexShader, m_fragmentShader, m_geometryShader, m_shaderProgram))
    {
        return false;
    }

    // uniformブロックへのリンク
    // 行列 (ビュー・プロジェクション)
    unsigned int uniformBlockIndex = glGetUniformBlockIndex(m_shaderProgram, "Matrices");
    glUniformBlockBinding(m_shaderProgram, uniformBlockIndex, 0);
    // カメラ関係 (座標、向き)
    uniformBlockIndex = glGetUniformBlockIndex(m_shaderProgram, "CameraVariable");
    glUniformBlockBinding(m_shaderProgram, uniformBlockIndex, 1);
    // カメラ関係 (座標、向き)
    uniformBlockIndex = glGetUniformBlockIndex(m_shaderProgram, "Triggers");
    glUniformBlockBinding(m_shaderProgram, uniformBlockIndex, 2);
    // カメラ関係 (座標、向き)
    uniformBlockIndex = glGetUniformBlockIndex(m_shaderProgram, "DirLight");
    glUniformBlockBinding(m_shaderProgram, uniformBlockIndex, 3);

    return true;
}

/// <summary>
/// 解放処理
/// </summary>
void GLSLprogram::Delete()
{
    // シェーダープログラムの解放
    glDeleteProgram(m_shaderProgram);
}

/// <summary>
/// シェーダーファイルを読み込み、コンパイルする
/// </summary>
/// <param name="_shaderPath"> コンパイルするシェーダーファイル </param>
/// <param name="_shaderType"> シェーダーの種類 (頂点、フラグメントetc...) </param>
/// <param name="_outShader"> コンパイルしたシェーダーを格納する変数 </param>
/// <returns> コンパイル成功したらtrueを返す </returns>
bool GLSLprogram::CompileShaderFromFile(const std::string& _shaderPath, GLenum _shaderType, GLuint& _outShader)
{

    // シェーダーファイルをオープン
    std::ifstream shaderFile;
    shaderFile.open(_shaderPath.c_str(), std::ios::in);
    if (shaderFile.is_open())
    {
        // 読み込んだシェーダーファイルの文字列を変換
        std::stringstream transText;
        transText << shaderFile.rdbuf();
        
        std::string contents = transText.str(); 
        const char* contentsChar = contents.c_str();
        shaderFile.close();

        // 引数に応じたタイプのシェーダーIDを作成・保管
        _outShader = glCreateShader(_shaderType);

        // 上で変換した文字列を用いてシェーダーをコンパイルする
        glShaderSource(_outShader, 1, &contentsChar, NULL);
        glCompileShader(_outShader);

        // 正常にコンパイルできたかチェック
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
/// シェーダープログラムへのリンクを行う関数
/// </summary>
/// <returns> リンクされたらtrueを返す </returns>
bool GLSLprogram::LinkShaders(const unsigned int& _vertShader, const unsigned int& _fragShader, const unsigned int& _geomShader, unsigned int& _program)
{
    // シェーダープログラムの作成
    m_shaderProgram = glCreateProgram();

    // シェーダープログラムに頂点シェーダー、フラグメントシェーダーをアタッチ
    // ※シェーダーリンクに必要な事前準備
    glAttachShader(m_shaderProgram, _vertShader);
    glAttachShader(m_shaderProgram, _fragShader);

    if (_geomShader != 0)
    {
        glAttachShader(m_shaderProgram, _geomShader);
    }


    // シェーダープログラムをリンク
    glLinkProgram(_program);

    // 正常にリンクできたかチェック
    if (!IsLinkProgram())
    {
        std::cout << "Failed::Shader::Link" << std::endl;

        return false;
    }


    return true;
}


/// <summary>
/// シェーダーが正常にコンパイルされたかを検証する
/// </summary>
/// <param name="_shader"> 検証したいシェーダー </param>
/// <returns> 正常にコンパイルされていた場合はtrueを返す </returns>
bool GLSLprogram::IsCompiled(unsigned int _shader)
{
    GLint status;

    // コンパイルの状態を問い合わせる
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
/// シェーダープログラムがリンクされたかを検証する
/// </summary>
/// <returns> 異常がなければtrueを返す </returns>
bool GLSLprogram::IsLinkProgram()
{
    int status;

    // シェーダープログラムのリンク状態を問い合わせる
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
/// シェーダープログラムのみを使用する
/// </summary>
void GLSLprogram::UseProgram()
{
    glUseProgram(m_shaderProgram);
}

//--------------------------------------------------------------+
// uniformセット関数群
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
