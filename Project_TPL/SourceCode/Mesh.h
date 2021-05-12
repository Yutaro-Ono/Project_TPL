//----------------------------------------------------------------------------------+
// @file        Mesh.h
// @brief       メッシュ情報(頂点配列・テクスチャ)を保持する
//              
// @note        親クラス (子クラスはファイル形式毎に定義)
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/7   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include "rapidjson/document.h"
#include "tinyObj/tiny_obj_loader.h"

// テクスチャの種類 (PBRマテリアル)
enum class PBR_MATERIAL : unsigned char
{
	ALBEDO,
	NORMAL,
	METALLIC,
	ROUGHNESS,
	AO,
	ALL
};

class Mesh
{

public:

	Mesh();
	virtual ~Mesh();

	virtual bool Load(const std::string& _filePath) = 0;
	
	void AddTextureStage(const std::string& _fileName);
	int CreateTextureStage(PBR_MATERIAL _type, const std::string& _fileName);


private:

	unsigned int m_vao;
	unsigned int m_vbo;

	class VertexArray* m_vertexArray;

	std::unordered_map<PBR_MATERIAL, int> m_textureStages;

};