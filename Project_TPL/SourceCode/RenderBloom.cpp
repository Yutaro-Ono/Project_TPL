//----------------------------------------------------------------------------------+
// @file        RenderBloom.cpp
// @brief       ブルーム描画処理
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 6/04   新規作成
//----------------------------------------------------------------------------------+
#include "RenderBloom.h"
#include "Renderer.h"
#include "GameMain.h"
#include "GameSettings.h"
#include "GLSLprogram.h"
#include "VertexArray.h"
#include "RenderBloomDebugObject.h"
#include "Debugger.h"

// 縮小バッファの段階数 (1/2, 1/4, 1/8, 1/16, 1/32の5段階)
const unsigned int maxLevelNum = 5;

RenderBloom::RenderBloom()
	:m_exposure(1.0f)
	,m_gamma(2.2f)
{
	CreateBlurFBO();

#ifdef _DEBUG

	CreateDebugObject();

#endif
}

RenderBloom::~RenderBloom()
{
}

void RenderBloom::CreateDebugObject()
{
	m_debugBloom = new RenderBloomDebugObject(this);
	DEBUGGER->AddDebugObject(m_debugBloom, OBJECT_TAG::SYSTEM);
}

/// <summary>
/// Step 1 :
/// 高輝度バッファを縮小バッファに段階保存する
/// </summary>
/// <param name="_brightBuffer"> 縮小保存する高輝度バッファ </param>
/// <param name="_downSampleShader"> ダウンサンプリング用シェーダー </param>
/// <param name="_screenVA"> スクリーン描画用頂点配列 </param>
void RenderBloom::DownSampling(unsigned int _brightBuffer, GLSLprogram* _downSampleShader, VertexArray* _screenVA)
{
	// 深度テストをオフ
	glDisable(GL_DEPTH_TEST);

	// 縮小バッファに描画するターゲット
	unsigned int renderSource = _brightBuffer;

	int reduceX = GAME_CONFIG.GetScreenSizeW();
	int reduceY = GAME_CONFIG.GetScreenSizeH();

	// 縮小バッファの奇数番目にダウンサンプリング結果を出力する
	for (int i = 0; i < maxLevelNum; i++)
	{
		// 縮小バッファの縦横サイズを更新
        // 1/2, 1/4, 1/8, 1/16, 1/32...
		reduceX /= 2;
		reduceY /= 2;

		// 縮小バッファに奇数番目を指定
		glBindFramebuffer(GL_FRAMEBUFFER, m_blurFBOs[i * 2 + 1]);
		// Viewportの調整
		glViewport(0, 0, reduceX, reduceY);
		// バッファのクリア
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// ターゲットした高輝度バッファをシェーダー内でダウンサンプリングし出力
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, renderSource);
		_downSampleShader->UseProgram();
		_downSampleShader->SetUniform("u_scene", 0);

		// スクリーンとして描画
		_screenVA->SetActive();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// 縮小バッファにターゲットを更新
		renderSource = m_blurTextures[i * 2 + 1];
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

/// <summary>
/// Step 2 :
/// 縮小バッファにガウスぼかしを施す
/// </summary>
/// <param name="_brightBuffer"> 高輝度バッファ </param>
/// <param name="_gaussShader"> ガウスぼかし用シェーダー </param>
/// <param name="_screenVA"> スクリーン描画用頂点配列 </param>
void RenderBloom::GaussBlur(GLSLprogram* _gaussShader, VertexArray* _screenVA)
{
	const int sampleCount = 15;
	Vector3 offset[sampleCount];
	int reduceX = GAME_CONFIG.GetScreenSizeW();
	int reduceY = GAME_CONFIG.GetScreenSizeH();
	float deviation = 2.0f;

	// ターゲットする縮小バッファ
	unsigned int renderSource = m_blurTextures[1];

	// ガウスぼかし段階数分ループ (5段階)
	for (int i = 0; i < maxLevelNum; i++)
	{
		// 縮小バッファの縦横サイズを更新
		reduceX /= 2;
		reduceY /= 2;

		bool h = false;
		// horizontal : 0 = 水平, 1 = 垂直方向にガウスぼかしをかける
		for (int horizontal = 0; horizontal < 2; horizontal++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_blurFBOs[i * 2 + horizontal]);
			// ビューポートの更新
			glViewport(0, 0, reduceX, reduceY);
			//glm::vec2 dir(1.0f - (float)horizontal, (float)horizontal);

			Vector2 dir;
			if (horizontal)
			{
				dir = Vector2(0.0f, 1.0f);
			}
			else
			{
				dir = Vector2(1.0f, 0.0f);
			}

			// ガウスぼかし計算
			CalcGaussBlurParam(reduceX, reduceY, dir, deviation, offset);

			// カラーバッファのクリア
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// ガウスぼかし処理を施して描画
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, renderSource);
			_gaussShader->UseProgram();
			_gaussShader->SetUniform("u_blurSource", 0);
			_gaussShader->SetUniform("u_gaussParam.sampleCount", sampleCount);

			// シェーダーにオフセット情報を送信
			for (int i = 0; i < sampleCount; i++)
			{
				// uniformにセット
				std::string uStr = "u_gaussParam.offset[" + std::to_string(i) + "]";
				_gaussShader->SetUniform(uStr.c_str(), offset[i]);
			}
			// スクリーンに描画
			_screenVA->SetActive();
			glDrawArrays(GL_TRIANGLES, 0, 6);

			// ターゲットにするバッファを更新
			renderSource = m_blurTextures[i * 2 + horizontal];
		}
		// ループ数ごとに偏差をインクリメント
		deviation += 1.5f;
	}

	// バインド解除・ビューポートを通常スクリーンサイズに変更
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH());

}

/// <summary>
/// Step3 :
/// 最終出力画面にブルーム効果を合成する
/// </summary>
/// <param name="_blendBuffer"> ブルーム合成したい描画バッファ </param>
/// <param name="_bloomShader"> ブルーム効果シェーダー </param>
/// <param name="_screenVA"> スクリーン描画用頂点配列 </param>
void RenderBloom::DrawBlendBloom(unsigned int _blendBuffer, GLSLprogram* _bloomShader, VertexArray* _screenVA)
{
	// 深度テストをオフ
	glDisable(GL_DEPTH_TEST);
	// バッファのカラー要素をクリア
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// 合成したいバッファをサンプリングする
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _blendBuffer);
	_bloomShader->UseProgram();
	_bloomShader->SetUniform("u_exposure", m_exposure);
	_bloomShader->SetUniform("u_gamma", m_gamma);
	_bloomShader->SetUniform("u_scene", 0);
	// 縮小バッファの段階毎にブルーム効果をかける
	for (unsigned int i = 0; i < maxLevelNum; i++)
	{
		int num = i + 1;
		glActiveTexture(GL_TEXTURE0 + num);
		glBindTexture(GL_TEXTURE_2D, m_blurTextures[i * 2 + 1]);
		std::string str = "u_bloom" + std::to_string(num);
		_bloomShader->SetUniform(str.c_str(), num);

	}

	// スクリーンに描画
	_screenVA->SetActive();
	glDrawArrays(GL_TRIANGLES, 0, 6);

}

/// <summary>
/// ガウスぼかし計算
/// </summary>
/// <param name="_w"></param>
/// <param name="_h"></param>
/// <param name="_dir"></param>
/// <param name="_deviation"></param>
/// <param name="_offset"></param>
void RenderBloom::CalcGaussBlurParam(int _w, int _h, Vector2 _dir, float _deviation, Vector3* _offset)
{
	float tu = 1.0f / float(_w);
	float tv = 1.0f / float(_h);
	_offset[0].x = 0.0f;
	_offset[0].y = 0.0f;
	_offset[0].z = GaussianDistribution(Vector2(0.0f, 0.0f), _deviation);
	float totalWeight = _offset[0].z;

	for (auto i = 0; i < 8; ++i)
	{
		int nextPos = (i - 1) * 2 + 1;
		_offset[i].x = _dir.x * tu * nextPos;
		_offset[i].y = _dir.y * tv * nextPos;
		_offset[i].z = GaussianDistribution(_dir * float(nextPos), _deviation);
		totalWeight += _offset[i].z * 2.0f;
	}
	for (auto i = 0; i < 8; ++i)
	{
		_offset[i].z /= totalWeight;
	}
	for (auto i = 8; i < 15; ++i)
	{
		_offset[i].x = -_offset[i - 7].x;
		_offset[i].y = -_offset[i - 7].y;
		_offset[i].z = _offset[i - 7].z;
	}
}

/// <summary>
/// ガウスぼかしの重みの計算
/// </summary>
/// <param name="_pos"> 座標 </param>
/// <param name="_rho"> 偏差 </param>
/// <returns></returns>
float RenderBloom::GaussianDistribution(const Vector2& _pos, float _rho)
{
	return exp(-(_pos.x * _pos.x + _pos.y * _pos.y) / (2.0f * _rho * _rho));
}

/// <summary>
/// ダウンサンプリング・ぼかし用のフレームバッファを作成する　(川瀬式ブルームフィルタを採用)
/// ガウスぼかしにて縦横2枚ずつ使用するので、段階ごとに2枚ずつ作成 (計5段階 x 2)
/// </summary>
void RenderBloom::CreateBlurFBO()
{
	// スクリーンの縦横サイズ (=テクスチャの縦横サイズ)
	int w = GAME_CONFIG.GetScreenSizeW();
	int h = GAME_CONFIG.GetScreenSizeH();

	// 段階数分ループ
	for (unsigned int i = 0; i < maxLevelNum; i++)
	{
		// 縮小バッファの縦横サイズを更新
		// 1/2, 1/4, 1/8, 1/16, 1/32...
		w /= 2;
		h /= 2;

		// 縮小バッファとテクスチャIDの領域を確保する
        // 縮小バッファ5段階 x 縦横2枚
		if (m_blurFBOs.empty())
		{
			m_blurFBOs.resize(maxLevelNum * 2);
		}
		if (m_blurTextures.empty())
		{
			m_blurTextures.resize(maxLevelNum * 2);
		}

		// 各段階ごとに縦横2枚分ループ (0 = 横, 1 = 縦)
		for (unsigned int j = 0; j < 2; j++)
		{

			// 縮小バッファの作成・縮小バッファテクスチャの作成
			glGenFramebuffers(1, &m_blurFBOs[i * 2 + j]);
			glGenTextures(1, &m_blurTextures[i * 2 + j]);
			glBindFramebuffer(GL_FRAMEBUFFER, m_blurFBOs[i * 2 + j]);
			glBindTexture(GL_TEXTURE_2D, m_blurTextures[i * 2 + 1]);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_blurTextures[i * 2 + j], 0);
			// バインド解除
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}
}
