//----------------------------------------------------------------------------------+
// @file        MeshObj.cpp
// @brief       メッシュ情報(頂点配列・テクスチャ)を保持する
//              
// @note        .obj形式ファイルの読み込みに対応
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/12   新規作成
//----------------------------------------------------------------------------------+
#include "MeshObj.h"
#include "VertexArray.h"

MeshObj::MeshObj()
{
}

MeshObj::~MeshObj()
{
}

/// <summary>
/// .objファイルのロード
/// </summary>
/// <param name="_filePath"> メッシュファイルパス </param>
/// <returns> 問題なくロードできたか </returns>
bool MeshObj::Load(const std::string& _filePath)
{
	Vertex::LAYOUT_TYPE layout = Vertex::LAYOUT_TYPE::POS_NORMAL_UV;

	tinyobj::attrib_t attrib;                      // 座標コンテナ
	std::vector<tinyobj::shape_t> shapes;          // 形状コンテナ
	std::vector<tinyobj::material_t> materials;    // マテリアルコンテナ

	// objファイルのロード
	std::string warning, error;
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, _filePath.c_str()))
	{
		std::cout << "ERROR::Load Obj Mesh::" << error << std::endl;
		return false;
	}
	
	// 各種頂点情報の個数
	const int attribVertNum = 3;
	const int attribNormNum = 3;
	const int attribUVNum = 2;
	// タンジェント空間 (ノーマルマップで使用)
	const int attribTanNum = 3;

	// ストライドの定義
	const int attribStride = attribVertNum + attribNormNum + attribUVNum + attribTanNum;

	// 頂点数の計算 (頂点座標の合計 / ポリゴンの頂点数(3))
	int vertNum = attrib.vertices.size() / attribVertNum;

	// インデックスバッファの取得
	std::vector<unsigned int> indices;
	// 形状ぶんループ
	for (const auto& shape : shapes)
	{
		// インデックスの取得
		for (const auto& index : shape.mesh.indices)
		{
			indices.push_back(index.vertex_index);
		}
	}

	// 頂点データの配列を確保 (頂点データ数 * ストライド数)
	std::vector<float> vertexVec(vertNum * attribStride);

	// タンジェント空間計算用
	


	return true;
}
