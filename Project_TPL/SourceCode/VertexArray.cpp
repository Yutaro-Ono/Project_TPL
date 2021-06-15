//----------------------------------------------------------------------------------+
// @file        VertexArray.h
// @brief       頂点配列をVAO/VBOに登録する
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/16   新規作成
//----------------------------------------------------------------------------------+
#include "VertexArray.h"


VertexArray::VertexArray()
{
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="_verts"> 頂点配列 </param>
/// <param name="_vertsNum"> 頂点数 </param>
/// <param name="_layout"> 頂点レイアウトの種類 </param>
/// <param name="_inDices"> インデックスバッファ </param>
/// <param name="_numInDices"> インデックス数 </param>
VertexArray::VertexArray(const void* _verts, unsigned int _vertsNum, VERTEX_LAYOUT::TYPE _layout, const unsigned int* _inDices, unsigned int _numInDices)
	:m_vertsNum(_vertsNum)
	,m_numInDices(_numInDices)
{
	// 頂点配列オブジェクトの登録・バインド
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// 頂点データサイズの計算 (計算式についてはLayoutの定義の部分に明記)
	unsigned vertexSize = 8 * sizeof(float);    // POS_NORMAL_

	if (_layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_UV_TAN)
	{
		vertexSize = 11 * sizeof(float);
	}
	if (_layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_SKIN_UV)
	{
		vertexSize = 8 * sizeof(float) + 8 * sizeof(char);
	}
	if (_layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_SKIN_UV_TAN)
	{
		vertexSize = 11 * sizeof(float) + 8 * sizeof(char);
	}

	// 頂点バッファオブジェクトの生成
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, _vertsNum * vertexSize, _verts, GL_STATIC_DRAW);

	// インデックスバッファの生成
	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numInDices * sizeof(unsigned int), _inDices, GL_STATIC_DRAW);

	// 頂点属性ごとに処理を派生
	if (_layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_UV)
	{
		// float 3個分　→　位置 x,y,z　位置属性をセット
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
		// 次のfloat 3個分 → 法線 nx, ny, nz　法線属性をセット
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 3));
		// 次のfloat 2個分 u, v  テクスチャ座標属性をセット
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6));
	}
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="_verts"> 頂点配列 </param>
/// <param name="_vertsNum"> 頂点数 </param>
/// <param name="_inDices"> インデックスバッファ </param>
/// <param name="_numInDices"> インデックス数 </param>
VertexArray::VertexArray(const float* _verts, unsigned int _vertsNum, const unsigned int* _inDices, unsigned int _numInDices)
{
}


VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_indexBuffer);
	glDeleteVertexArrays(1, &m_vao);
}

/// <summary>
/// 頂点配列オブジェクトをバインドし、描画の準備をする
/// </summary>
void VertexArray::SetActive()
{
	glBindVertexArray(m_vao);
}

/// <summary>
/// シンプルな四角形 (平面)
/// スクリーンに使用
/// </summary>
void VertexArray::CreateSimpleQuadVAO()
{
	float quadVertices[] =
	{
		// ポジション   // テクスチャ座標
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}
