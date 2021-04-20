//----------------------------------------------------------------------------------+
// @file        VertexArray.h
// @brief       頂点配列をVAO/VBOに登録する
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/16   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

namespace Vertex
{
	//-------------------------------------------------------------------------------------------------------------------+
    // 頂点レイアウトのデータサイズ
    // POS_NORMAL_UV = 8 * sizeof(float) = 32 bytes
    // | POSITION  | NORMAL    | TEXTURE |
    // | x | y | z | x | y | z | u  | v  | 
    //
	// ※タンジェント空間は通常の座標と同じ <3 * sizeof(float) = 12byte>
	//
    // POS_NORMAL_SKIN_UV
    // POS_NORMAL_SKIN_UV = (8 * sizeof(float)) + (8 * sizeof(char)) = 40 bytes
    // | POSITION  | NORMAL    | BONES   | WEIGHT  |TEXTURE|
    // | x | y | z | x | y | z | char[4] | char[4] | u | v |
    //                                    ※WEIGHTの確保はcharだが、精度が必要ないので8bit固定小数として使用する
    //-------------------------------------------------------------------------------------------------------------------+
	// 頂点レイアウト (アトリビュートに登録する頂点のデータ形式)
	enum LAYOUT_TYPE
	{

		POS_NORMAL_UV,                     // 座標、法線、UV座標
		POS_NORMAL_UV_TAN,                 // 座標、法線、UV座標、タンジェント空間
		POS_NORMAL_SKIN_UV,                // 座標、法線、UV座標、スキンメッシュのボーンと重み情報
		POS_NORMAL_SKIN_UV_TAN             // 座標、法線、UV座標、スキンメッシュのボーンと重み情報、タンジェント空間

	};
}


class VertexArray
{

public:

	VertexArray();
	VertexArray(const void* _verts, unsigned int _vertsNum, Vertex::LAYOUT_TYPE _layout,
		                        const unsigned int* _inDices, unsigned int _numInDices);

	VertexArray(const float* _verts, unsigned int _vertsNum,
		        const unsigned int* _inDices, unsigned int _numInDices);

	~VertexArray();


private:


	GLuint m_vertsNum;                          // 頂点数
	GLuint m_numInDices;                        // インデックス数
	GLuint m_vao;                               // 頂点配列オブジェクト(ID)
	GLuint m_vbo;                               // 頂点バッファオブジェクト(ID)
	GLuint m_indexBuffer;                       // インデックスバッファ(ID)

};