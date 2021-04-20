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
VertexArray::VertexArray(const void* _verts, unsigned int _vertsNum, Vertex::LAYOUT_TYPE _layout, const unsigned int* _inDices, unsigned int _numInDices)
	:m_vertsNum(_vertsNum)
	,m_numInDices(_numInDices)
{
	// 頂点配列オブジェクトの登録・バインド
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// 頂点データサイズの計算 (計算式についてはLayoutの定義の部分に明記)
	unsigned vertexSize = 8 * sizeof(float);    // POS_NORMAL_

	if (_layout == Vertex::LAYOUT_TYPE::POS_NORMAL_UV_TAN)
	{
		vertexSize = 11 * sizeof(float);
	}
	if (_layout == Vertex::LAYOUT_TYPE::POS_NORMAL_SKIN_UV)
	{
		vertexSize = 8 * sizeof(float) + 8 * sizeof(char);
	}
	if (_layout == Vertex::LAYOUT_TYPE::POS_NORMAL_SKIN_UV_TAN)
	{
		vertexSize = 11 * sizeof(float) + 8 * sizeof(char);
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
