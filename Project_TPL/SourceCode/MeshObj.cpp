//----------------------------------------------------------------------------------+
// @file        MeshObj.cpp
// @brief       メッシュ情報(頂点配列・テクスチャ)を保持する
//              
// @note        .obj形式ファイルの読み込みに対応
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/12   新規作成
// 2021/ 5/14   メッシュの読み込み処理が完成
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
	VERTEX_LAYOUT::TYPE layout = VERTEX_LAYOUT::TYPE::POS_NORMAL_UV;

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


	// ストライドの定義
	int attribStride = attribVertNum + attribNormNum + attribUVNum;
	// タンジェント情報が必要な場合はストライドを拡張
	if (layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_UV_TAN || layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_SKIN_UV_TAN)
	{
		// タンジェント空間 (ノーマルマップで使用)
		const int attribTanNum = 3;

		attribStride += attribTanNum;
	}


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
	glm::vec3 tangent;
	// 頂点座標・テクスチャUV格納用
	std::vector<glm::vec3> destPos(3);
	std::vector<glm::vec2> uvPos(3);

	// 形状ループ
	for (const auto& shape : shapes)
	{
		size_t indexOffset = 0;

		// 面数ループ
		for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
		{
			size_t num_vertices = shape.mesh.num_face_vertices[f];

			// 頂点数ループ (1面に3回)
			for (size_t v = 0; v < num_vertices; v++)
			{

				// 面を構成するインデックスを取得
				tinyobj::index_t idx = shape.mesh.indices[indexOffset + v];

				// 頂点座標を頂点配列にコピー
				vertexVec[idx.vertex_index * attribStride + 0] = attrib.vertices[3 * idx.vertex_index + 1];
				vertexVec[idx.vertex_index * attribStride + 1] = attrib.vertices[3 * idx.vertex_index + 0];
				vertexVec[idx.vertex_index * attribStride + 2] = attrib.vertices[3 * idx.vertex_index + 2];

				// 法線データを頂点配列にコピー
				vertexVec[idx.vertex_index * attribStride + 3] = attrib.vertices[3 * idx.normal_index + 0];
				vertexVec[idx.vertex_index * attribStride + 4] = attrib.vertices[3 * idx.normal_index + 1];
				vertexVec[idx.vertex_index * attribStride + 5] = attrib.vertices[3 * idx.normal_index + 2];

				// uvデータを頂点配列にコピー
				vertexVec[idx.vertex_index * attribStride + 6] = attrib.texcoords[2 * idx.texcoord_index + 0];
				vertexVec[idx.vertex_index * attribStride + 7] = 1.0f - attrib.texcoords[2 * idx.texcoord_index + 1];


				//-------------------------------------------------------+
				// 当たり判定ボックス・半径の算出
				//-------------------------------------------------------+
				// 



				// ポリゴンを構成する頂点座標を一時保存
				destPos[v] = glm::vec3(vertexVec[(int)idx.vertex_index * attribStride + 0],
					                   vertexVec[(int)idx.vertex_index * attribStride + 1],
					                   vertexVec[(int)idx.vertex_index * attribStride + 2]);

				// テクスチャ座標を一時保存
				uvPos[v] = glm::vec2(vertexVec[(int)idx.vertex_index * attribStride + 6],
					                 vertexVec[(int)idx.vertex_index * attribStride + 7]);

			}

			// タンジェント空間情報が必要なモデルのみ
			if (layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_UV_TAN || layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_SKIN_UV_TAN)
			{
				// タンジェントの算出
				CalcTangentVec(tangent, destPos[0], destPos[1], destPos[2], uvPos[0], uvPos[1], uvPos[2]);
				// タンジェント情報を頂点配列に格納
				for (int i = 0; i < 3; i++)
				{
					SetTangent(vertexVec, indexOffset + i, tangent);
				}
			}


			// 基準インデックスを変更
			indexOffset += num_vertices;

		}
	}

	// 読み込んだ頂点配列情報をGLに登録する
	m_vertexArray = new VertexArray( static_cast<float*>(vertexVec.data()), static_cast<unsigned>(vertNum),
		                             layout, indices.data(), static_cast<unsigned>(indices.size()) );

	// テクスチャの追加
	AddTextureStage(_filePath);


	return true;
}
