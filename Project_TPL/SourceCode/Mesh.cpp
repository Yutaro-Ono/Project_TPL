//----------------------------------------------------------------------------------+
// @file        Mesh.cpp
// @brief       メッシュ情報(頂点配列・テクスチャ)を保持する
//              
// @note        親クラス (子クラスはファイル形式毎に定義)
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/7   新規作成
//----------------------------------------------------------------------------------+
#include "Mesh.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "TexturePool.h"
#include "VertexArray.h"

const char* PBR_MATERIAL_NAME[static_cast<int>(PBR_MATERIAL::ALL)] =
{
	"_Albedo",
	"_Normal",
	"_Metallic",
	"_Roughness",
	"_AO",
	"_Specular",
	"_Emissive"
};

Mesh::Mesh()
	:m_vertexArray(nullptr)
{
}

Mesh::~Mesh()
{
	delete m_vertexArray;
}


/// <summary>
/// テクスチャステージの追加処理 (各種テクスチャ1つずつ)
/// メッシュのファイル名から拡張子部分を除く ※ building.obj → building
/// </summary>
/// <param name="_fileName"> メッシュファイルの名前 </param>
void Mesh::AddTextureStage(const std::string& _fileName)
{
	// ファイル拡張子前の "." が何文字目にあるかを取得する
	int extNum = _fileName.find_last_of(".");
	// 拡張子を除いたファイル名のみを取得
	std::string path = _fileName.substr(0, extNum);

	// 各種テクスチャの取得 (1つずつ)
	m_textureStages.emplace(PBR_MATERIAL::ALBEDO, CreateTextureStage(PBR_MATERIAL::ALBEDO, path));
	m_textureStages.emplace(PBR_MATERIAL::NORMAL, CreateTextureStage(PBR_MATERIAL::NORMAL, path));
	m_textureStages.emplace(PBR_MATERIAL::METALLIC, CreateTextureStage(PBR_MATERIAL::METALLIC, path));
	m_textureStages.emplace(PBR_MATERIAL::ROUGHNESS, CreateTextureStage(PBR_MATERIAL::ROUGHNESS, path));
	m_textureStages.emplace(PBR_MATERIAL::AO, CreateTextureStage(PBR_MATERIAL::AO, path));
	m_textureStages.emplace(PBR_MATERIAL::SPECULAR, CreateTextureStage(PBR_MATERIAL::SPECULAR, path));
	m_textureStages.emplace(PBR_MATERIAL::EMISSIVE, CreateTextureStage(PBR_MATERIAL::EMISSIVE, path));
}

/// <summary>
/// テクスチャステージの登録処理
/// </summary>
/// <param name="_type"> PBRマテリアルの種類 </param>
/// <param name="_filePath"> ファイルの名前 </param>
/// <returns> テクスチャID </returns>
int Mesh::CreateTextureStage(PBR_MATERIAL _type, const std::string& _fileName)
{

	// テクスチャタイプごとにstringをセット
	std::string texturePath = _fileName + PBR_MATERIAL_NAME[static_cast<int>(_type)] + ".png";

	// テクスチャクラスのポインタをプールから取得または生成
	Texture* t = nullptr;
	t = TEXTURE_POOL->GetTexture(texturePath);
	
	// テクスチャの取得に成功したらIDを返す
	if (t != nullptr)
	{
		return t->GetTextureID();
	}

	// テクスチャがロードできなかった場合は0を返す
	return 0;
}

/// <summary>
/// 頂点配列オブジェクトのバインド
/// </summary>
void Mesh::SetActiveVAO()
{
	m_vertexArray->SetActive();
}

/// <summary>
/// タンジェントベクトル計算
/// </summary>
/// <param name="_destTangent"> 計算結果を保存する3次元ベクトルクラス </param>
/// <param name="_pos1"></param>
/// <param name="_pos2"></param>
/// <param name="_pos3"></param>
/// <param name="_uv1"></param>
/// <param name="_uv2"></param>
/// <param name="_uv3"></param>
void Mesh::CalcTangentVec(glm::vec3& destTangent_, const glm::vec3& _pos1, const glm::vec3& _pos2, const glm::vec3& _pos3, const glm::vec2& _uv1, const glm::vec2& _uv2, const glm::vec2& _uv3)
{
	glm::vec3 edge1 = _pos2 - _pos1;
	glm::vec3 edge2 = _pos3 - _pos1;

	glm::vec2 deltaUV1 = _uv2 - _uv1;
	glm::vec2 deltaUV2 = _uv3 - _uv1;
	
	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
	
	destTangent_.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	destTangent_.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	destTangent_.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	
	glm::normalize(destTangent_);
}

/// <summary>
/// 頂点配列にタンジェント情報を書き込む
/// </summary>
/// <param name="_destfv"> タンジェント情報を格納する配列 </param>
/// <param name="index"> 配列の書き込み先インデックス </param>
/// <param name="_tangent"> 配列に書き込むタンジェント情報 </param>
void Mesh::SetTangent(std::vector<float>& destfv_, int _index, const glm::vec3& _tangent)
{
	if (destfv_.size() > _index * 11 + 8 + 2)
	{
		destfv_[_index * 11 + 8 + 0] = _tangent.x;
		destfv_[_index * 11 + 8 + 1] = _tangent.y;
		destfv_[_index * 11 + 8 + 2] = _tangent.z;
	}
}
