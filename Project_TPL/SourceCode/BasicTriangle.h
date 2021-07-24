//----------------------------------------------------------------------------------+
// @file        BasicTriangle.h
// @brief       簡単な三角形オブジェクトの作成・描画オブジェクトとして登録
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/16   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include <string>
#include "Math.h"

class BasicTriangle
{

public:

	BasicTriangle();
	~BasicTriangle();

	void Draw(class GLSLprogram* _shader);

	unsigned int GetTriangleVAO() { return m_vao; }     // 三角形用頂点配列オブジェクトのゲッター

	void SetTriangleColor(const Vector3& _color) { m_color = _color; }


private:

	// 頂点バッファ・配列用オブジェクト
	unsigned int m_vbo;
	unsigned int m_vao;

	class Texture* m_diffuseMap;

	// ワールド変換行列
	Matrix4 m_worldTrans;
	// 拡大率、回転値
	Vector3 m_scale;
	float m_radian;

	// カラー情報
	Vector3 m_color;

	static const std::string TEXTURE_KEY;

};