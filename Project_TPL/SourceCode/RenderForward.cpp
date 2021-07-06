#include "RenderForward.h"
#include "GameMain.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "GameSettings.h"
#include "ShaderManager.h"
#include "DrawableObjectManager.h"

RenderForward::RenderForward(Renderer* _renderer)
	:RenderMethodBase(_renderer)
{
}

RenderForward::~RenderForward()
{
}

bool RenderForward::Load()
{
	return CreateColorBuffer();
}

void RenderForward::Draw(ShaderManager* _shaderManager, DrawableObjectManager* _drawObjects)
{

	glBindFramebuffer(GL_FRAMEBUFFER, m_colorFBO);

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);      // 指定した色値で画面をクリア
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);              // 画面のカラー・深度・ステンシルバッファをクリア

    // メッシュの描画
    glEnable(GL_DEPTH_TEST);
    _shaderManager->EnableShaderProgram(GLSLshader::BASIC_MESH);
    _drawObjects->Draw(_shaderManager->GetShader(GLSLshader::BASIC_MESH));

    // 法線の視覚化
    if (m_renderer->GetIsEnableVisualizeNormal())
    {
        _shaderManager->EnableShaderProgram(GLSLshader::OPTION_NORMAL_VISUALIZE);
        _drawObjects->Draw(_shaderManager->GetShader(GLSLshader::OPTION_NORMAL_VISUALIZE));
    }

	// ブルーム効果が有効でない場合
    // Gバッファ+ライトパスの結果をそのまま画面へ出力する
	if (m_renderer->GetIsEnableBloom())
	{
		m_renderer->BloomPass(m_emissive, m_albedoSpec);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_outScreenShader->UseProgram();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_albedoSpec);

		// スクリーンへ描画
		m_renderer->GetQuadVertex()->SetActive();
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


/// <summary>
/// フォワードレンダリング用カラーバッファの生成
/// </summary>
bool RenderForward::CreateColorBuffer()
{
	// Gバッファの生成・バインド
	glGenFramebuffers(1, &m_colorFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_colorFBO);

	//--------------------------------------------------------------------+
	// カラーバッファ各要素の登録
	//--------------------------------------------------------------------+
	// アルベド&スペキュラ情報バッファ (8bitカラーバッファ / 0番目)
	glGenTextures(1, &m_albedoSpec);
	glBindTexture(GL_TEXTURE_2D, m_albedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_albedoSpec, 0);
	// 高輝度バッファ (エミッシブバッファ / 1番目)
	glGenTextures(1, &m_emissive);
	glBindTexture(GL_TEXTURE_2D, m_emissive);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_emissive, 0);

	// 生成した各要素をGバッファの描画先としてGLに明示する
	m_attachments[0] = { GL_COLOR_ATTACHMENT0 };
	m_attachments[1] = { GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, m_attachments);

	// レンダーバッファの生成・登録
	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

	// フレームバッファの整合性チェック
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::ColorBuffer *ForwardRender::Create Failed" << std::endl;
		return false;
	}

	// バインドを解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}
