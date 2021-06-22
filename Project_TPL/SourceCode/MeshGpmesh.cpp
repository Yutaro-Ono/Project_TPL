//----------------------------------------------------------------------------------+
// @file        MeshGpmesh.cpp
// @brief       メッシュ情報(頂点配列・テクスチャ)を保持する
//              
// @note        .gpmesh形式ファイルの読み込みに対応
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/21   新規作成
//----------------------------------------------------------------------------------+
#include "MeshGpmesh.h"
#include <fstream>
#include <sstream>
#include "VertexArray.h"

MeshGpmesh::MeshGpmesh()
	:m_specular(1.0f)
{
}

MeshGpmesh::~MeshGpmesh()
{
}


/// <summary>
/// gpmeshファイルのロード
/// </summary>
/// <param name="_filePath"> 読み込むgpmeshファイルへのパス </param>
/// <returns> 正常に読み込めた場合はtrueを返す </returns>
bool MeshGpmesh::Load(const std::string& _filePath)
{
	// gpmeshファイルを開く
	std::ifstream file(_filePath);
	if (!file.is_open())
	{
		std::cout << "ERROR::GPMESH File Not Found::" << _filePath << std::endl;
		return false;
	}

	// streamに開いたgpmeshファイルを書き込む
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	// 更にstringに変換し、char形式としてrapidjsonのストリームに格納する
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStream(contents.c_str());
	// jsonドキュメントとして解析する
	rapidjson::Document doc;
	doc.ParseStream(jsonStream);

	// jsonオブジェクトとして解析できたか
	if (!doc.IsObject())
	{
		std::cout << "ERROR::Not valid json::" << _filePath << std::endl;
		return false;
	}

	// jsonバージョンチェック
	int ver = doc["version"].GetInt();
	if (ver != 1)
	{
		std::cout << "ERROE::GPMESH is not version 1 ::" << _filePath << std::endl;
		return false;
	}

	//----------------------------------------------------------------------------------------------------+
	// gpmeshの各要素を取得
	m_shaderName = doc["shader"].GetString();                               // シェーダー
	m_specular = static_cast<float>(doc["specularPower"].GetDouble());      // スペキュラ強度
	std::string vertexFormat = doc["vertexformat"].GetString();             // 頂点レイアウト
	//----------------------------------------------------------------------------------------------------+

	//----------------------------------------------------------------------------------------------------+
	// 頂点レイアウトと1頂点の情報量をセット
	// 基本 : 3 (位置xyz) + 3(法線xyz) ＋ 2(UV) の計 8個分（32byte)
	VERTEX_LAYOUT::TYPE layout = VERTEX_LAYOUT::TYPE::POS_NORMAL_UV;
	size_t vertexSize = 8;
	// タンジェント情報を持つモデル
	if (layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_UV_TAN)
	{
		vertexSize = 11;     // vx, vy, vz, nx, ny, nz, u, v, tx, ty, tz
	}
	// スキンメッシュ(タンジェントなし)
	// 3 (位置xyz) + 3(法線xyz) + 2(Boneと重み）＋　2(UV)  の計　10個分（40byte) 　
	if (layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_SKIN_UV)
	{
		vertexSize = 10;
	}
	// スキンメッシュ(タンジェントあり)
	if (layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_SKIN_UV_TAN)
	{
		vertexSize = 13;
	}
	//----------------------------------------------------------------------------------------------------+

	//----------------------------------------------------------------------------------------------------+
	// 頂点配列の読み込み
	// 配列化できなかった、または配列サイズを確保できなかった場合は失敗
	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1)
	{
		std::cout << "ERROR::GPMESH is no vertices::" << _filePath << std::endl;
		return false;
	}
	//----------------------------------------------------------------------------------------------------+

	//----------------------------------------------------------------------------------------------------+
	// 頂点インデックスの読み込み処理
	const rapidjson::Value& indicesJson = doc["indices"];
	if (!indicesJson.IsArray() || indicesJson.Size() < 1)
	{
		std::cout << "ERROR::GPMESH has no indices::" << _filePath << std::endl;
		return false;
	}
	// 頂点インデックスを配列に格納
	std::vector<unsigned int> indices;
	indices.reserve(indicesJson.Size() * 3);
	for (rapidjson::SizeType i = 0; i < indicesJson.Size(); i++)
	{
		const rapidjson::Value& index = indicesJson[i];
		if (!index.IsArray() || index.Size() != 3)
		{
			std::cout << "ERROR::GPMESH Invalid indices for " << _filePath << std::endl;
			return false;
		}

		indices.emplace_back(index[0].GetUint());
		indices.emplace_back(index[1].GetUint());
		indices.emplace_back(index[2].GetUint());
	}
	//----------------------------------------------------------------------------------------------------+

	//----------------------------------------------------------------------------------------------------+
	// 頂点データをfloat型配列として確保していく
	// 頂点データ格納用配列
	std::vector<Vertex> vertices;
	vertices.reserve(vertsJson.Size() * vertexSize);
	// タンジェント空間計算用
	std::vector<glm::vec3> destPos;
	std::vector<glm::vec2> uvPos;
	// m_radius = 0.0f;
	// 頂点情報読み込みループ
	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
	{
		// 1つの頂点情報が配列化しているかをチェック (1つの要素に複数のデータが入っているため)
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray())
		{
			std::cout << "ERROR::Unexpected vertex format for %s::" << _filePath << std::endl;
			return false;
		}

		//-------------------------------------------------+
		// 当たり判定計算

		//-------------------------------------------------+

		//-----------------------------------------------------------------------------+
		// 頂点レイアウト毎の読み込み処理
		Vertex v;
		// ボーン無しモデルデータ
		if (layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_UV || layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_UV_TAN)
		{
			// gpmeshファイルから頂点情報を確保
			for (rapidjson::SizeType j = 0; j < vert.Size(); j++)
			{
				
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
				
			}


			// 法線マップを適用する場合
			if (layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_UV_TAN)
			{
				// タンジェント計算用の頂点座標とUV座標を格納
				destPos.push_back(glm::vec3(vertices[i * 11 + 0].f, vertices[i * 11 + 1].f, vertices[i * 11 + 2].f));
				uvPos.push_back(glm::vec2(vertices[i * 11 + 6].f, vertices[i * 11 + 7].f));
				// タンジェント空間用のデータ領域を確保
				for (int tan = 0; tan < 3; tan++)
				{
					v.f = 0.0f;
					vertices.emplace_back(v);
				}

				// 1ポリゴン分の情報が集まったらタンジェント計算を行い、頂点配列に追加する
				if (destPos.size() == 3)
				{
					glm::vec3 tangent;
					CalcTangentVec(tangent, destPos[0], destPos[1], destPos[2], uvPos[0], uvPos[1], uvPos[2]);
					// 頂点配列へ追加
					for (int tan = 2; tan >= 0; tan--)
					{
						vertices[(i - tan) * 11 + 8 + 0].f = static_cast<float>(tangent.x);
						vertices[(i - tan) * 11 + 8 + 1].f = static_cast<float>(tangent.y);
						vertices[(i - tan) * 11 + 8 + 2].f = static_cast<float>(tangent.z);
					}
					// 保存していた頂点座標・テクスチャ座標データをクリアしておく
					uvPos.clear();
					destPos.clear();
				}
			}
		}
		// ボーン入りモデルデータ
		else if (layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_SKIN_UV || layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_SKIN_UV_TAN)
		{
			// 頂点座標と法線を追加 vx, vy, vz, nx, ny, nz
			for (rapidjson::SizeType j = 0; j < 6; j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}

			// スキン情報の追加 (ボーン番号:unsigned charの1バイト分)
			// 2ループ (1:ボーン番号  2:ボーンウェイト)
			for (rapidjson::SizeType j = 6; j < 14; j += 4)
			{
				v.b[0] = vert[j].GetUint();
				v.b[1] = vert[j + 1].GetUint();
				v.b[2] = vert[j + 2].GetUint();
				v.b[3] = vert[j + 3].GetUint();
				vertices.emplace_back(v);
			}

			// テクスチャ座標の追加
			for (rapidjson::SizeType j = 14; j < vert.Size(); j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}

			if (layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_SKIN_UV_TAN)
			{
				// タンジェント計算用の頂点座標とUV座標を格納
				destPos.push_back(glm::vec3(vertices[i * 13 + 0].f, vertices[i * 13 + 1].f, vertices[i * 13 + 2].f));
				uvPos.push_back(glm::vec2(vertices[i * 13 + 8].f, vertices[i * 13 + 9].f));
				// タンジェント空間用のデータ領域を確保
				for (int tan = 0; tan < 3; tan++)
				{
					v.f = 0.0f;
					vertices.emplace_back(v);
				}

				// 1ポリゴン分の情報が集まったらタンジェント計算を行い、頂点配列に追加する
				if (destPos.size() == 3)
				{
					glm::vec3 tangent;
					CalcTangentVec(tangent, destPos[0], destPos[1], destPos[2], uvPos[0], uvPos[1], uvPos[2]);
					// 頂点配列へ追加
					for (int tan = 2; tan >= 0; tan--)
					{
						vertices[(i - tan) * 13 + 10 + 0].f = static_cast<float>(tangent.x);
						vertices[(i - tan) * 13 + 10 + 1].f = static_cast<float>(tangent.y);
						vertices[(i - tan) * 13 + 10 + 2].f = static_cast<float>(tangent.z);
					}
					// 保存していた頂点座標・テクスチャ座標データをクリアしておく
					uvPos.clear();
					destPos.clear();
				}
			}

		}
		// 頂点レイアウト毎の読み込み処理 ここまで
		//----------------------------------------------------------------------------+

	}

	// テクスチャステージの追加
	AddTextureStage(_filePath);

	// 頂点配列クラスの生成
	m_vertexArray = new VertexArray(vertices.data(), static_cast<unsigned>(vertices.size()) / vertexSize, 
		                            layout, indices.data(), static_cast<unsigned>(indices.size()));

	return true;
}
