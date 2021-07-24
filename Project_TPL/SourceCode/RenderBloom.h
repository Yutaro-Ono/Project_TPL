//----------------------------------------------------------------------------------+
// @file        RenderBloom.h
// @brief       ブルーム描画処理
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 6/04   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include <vector>
#include "Math.h"

class RenderBloom
{

public:

	RenderBloom();
	~RenderBloom();

	void CreateDebugObject();

	void DownSampling(unsigned int _brightBuffer, class GLSLprogram* _downSampleShader, class VertexArray* _screenVA);
	void GaussBlur(class GLSLprogram* _gaussShader, class VertexArray* _screenVA);
	void DrawBlendBloom(unsigned int _blendBuffer, class GLSLprogram* _bloomShader, class VertexArray* _screenVA);

	void CalcGaussBlurParam(int _w, int _h, Vector2 _dir, float _deviation, Vector3* _offset);
	float GaussianDistribution(const Vector2& _pos, float _rho);

private:

	void CreateBlurFBO();

	// 縮小バッファとテクスチャ配列
	std::vector<unsigned int> m_blurFBOs;
	std::vector<unsigned int> m_blurTextures;

	// トーンマッピング時の露出量
	float m_exposure;

	// ガンマコレクション
	float m_gamma;

	// デバッグクラス
	class RenderBloomDebugObject* m_debugBloom;
	friend class RenderBloomDebugObject;
};