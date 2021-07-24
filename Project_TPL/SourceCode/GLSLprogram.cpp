//----------------------------------------------------------------------------------+
// @file        GLSLprogram.cpp
// @brief       GLSLシェーダーのコンパイル・リンクを行う
// @note        シェーダープログラムの情報を保持
// @author      小野 湧太郎 (Yutaro Ono, @2021)
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
/// シェーダーの読み込み・コンパイル・リンク処理
/// </summary>
/// <param name="_vertPath"> 頂点シェーダーのファイルパス </param>
/// <param name="_fragPath"> フラグメントシェーダーのファイルパス </param>
/// <param name="_geomPath"> ジオメトリシェーダーのファイルパス </param>
/// <returns> ロード処理に成功したか(true)・失敗したか(false) </returns>
bool GLSLprogram::LoadShaders(const char* _vertPath, const char* _fragPath, const char* _geomPath)
{

    // ファイルパスからシェーダーソースコードを取得する
    std::string vertCode;
    std::string fragCode;
    std::string geomCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;

    // ifstreamオブジェクトが例外をスローできるか確認
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);


    try
    {
        // シェーダーファイルを開く
        vShaderFile.open(_vertPath);
        fShaderFile.open(_fragPath);

        // ファイル内のバッファを読み取り、streamに格納
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // ファイルのハンドルを閉じる
        vShaderFile.close();
        fShaderFile.close();

        // streamの内容をstringに変換
        vertCode = vShaderStream.str();
        fragCode = fShaderStream.str();

        // ジオメトリシェーダーが指定されていた場合
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

    // 文字列に保管
    const char* vShaderCode = vertCode.c_str();
    const char* fShaderCode = fragCode.c_str();

    // シェーダーのコンパイル
    unsigned int vertex, fragment;

    // 頂点シェーダー
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    
    // フラグメントシェーダー
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // 正常にコンパイルできたかチェック
    if (!CompileShaderFromFile(_vertPath, GL_VERTEX_SHADER, vertex) ||
        !CompileShaderFromFile(_fragPath, GL_FRAGMENT_SHADER, fragment))
    {
        return false;
    }

    // ジオメトリシェーダー
    unsigned int geometry = 0;
    if (_geomPath != nullptr)
    {

        const char* gShaderCode = geomCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);

        // 正常にコンパイルできたかチェック
        if (!CompileShaderFromFile(_geomPath, GL_GEOMETRY_SHADER, geometry))
        {
            return false;
        }
    }

    // シェーダープログラムへのリンク
    if (!LinkShaders(vertex, fragment, geometry, m_shaderProgram))
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
bool GLSLprogram::CompileShaderFromFile(const char* _shaderPath, GLenum _shaderType, unsigned int& _outShader)
{


    // シェーダーファイルをオープン
    std::ifstream shaderFile(_shaderPath);

    if (shaderFile.is_open())
    {
        // 読み込んだシェーダーファイルの文字列を変換
        std::stringstream transText;
        transText << shaderFile.rdbuf();

        std::string contents = transText.str();
        
        const char* contentsChar = contents.c_str();

        // 引数に応じたタイプのシェーダーIDを作成・保管
        _outShader = glCreateShader(_shaderType);

        // 上で変換した文字列を用いてシェーダーをコンパイルする
        glShaderSource(_outShader, 1, &(contentsChar), nullptr);
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
