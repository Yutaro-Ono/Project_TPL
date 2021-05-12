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


const char* PBR_MATERIAL_NAME[static_cast<int>(PBR_MATERIAL::ALL)] =
{
	"_ALBEDO",
	"_NORMAL",
	"_METALLIC",
	"_ROUGHNESS",
	"_AO"
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

}

/// <summary>
/// テクスチャステージの登録処理
/// </summary>
/// <param name="_type"> PBRマテリアルの種類 </param>
/// <param name="_filePath"> ファイルの名前 </param>
/// <returns> テクスチャID </returns>
int Mesh::CreateTextureStage(PBR_MATERIAL _type, const std::string& _fileName)
{
	// マテリアルタイプごとのテクスチャパスを作成
	std::string texturePath = _fileName + PBR_MATERIAL_NAME[static_cast<int>(_type)];

	// テクスチャの生成
	Texture* t = nullptr;
	t->LoadTexture(texturePath);

	// テクスチャの取得に成功したらIDを返す
	if (t != nullptr)
	{
		return t->GetTextureID();
	}


	return 0;
}
