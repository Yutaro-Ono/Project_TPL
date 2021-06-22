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
	unsigned vertexSize = 8 * sizeof(float);    // POS_NORMAL_UV

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

	// アトリビュートセット 共通
	// float 3個分　→　位置 x,y,z　位置属性をセット
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)0);
	// 次のfloat 3個分 → 法線 nx, ny, nz　法線属性をセット
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
		(void*)(3 * sizeof(float)));
	// 頂点属性ごとに処理を派生
	if (_layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_UV)
	{
		// 次のfloat 2個分 u, v  テクスチャ座標属性をセット
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize,
			(void*)(6 * sizeof(float)));
	}
	if (_layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_UV_TAN)
	{
		// 2 : テクスチャ座標
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*)(6 * sizeof(float)));
		// 3 : 接ベクトル (タンジェント)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)(8 * sizeof(float)));
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

/// <summary>
/// キューブマップ用頂点配列の作成
/// </summary>
void VertexArray::CreateCubeMapVertices()
{
	float cubeMapVertices[] = 
	{      
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	unsigned int indices[] =
	{
		0,  1,  2,  0,  2,  3,    // 前面
		4,  5,  6,  4,  6,  7,    // 背面
		8,  9, 10,  8, 10, 11,    // 上面
		12, 13, 14, 12, 14, 15,    // 底面
		16, 17, 18, 16, 18, 19,    // 右側面
		20, 21, 22, 20, 22, 23     // 左側面
	};


	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 36 * 8 * sizeof(float), cubeMapVertices, GL_STATIC_DRAW);
	// アトリビュートへのセット(頂点座標のみ)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}
