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
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "document.h"

// テクスチャの種類 (PBRマテリアル)
enum class PBR_MATERIAL : unsigned char
{
	ALBEDO = 0,
	NORMAL,
	METALLIC,
	ROUGHNESS,
	AO,
	SPECULAR,
	EMISSIVE,
	ALL
};

namespace
{
	union Vertex
	{
		float f;
		uint8_t b[4];
	};
}

class Mesh
{

public:

	Mesh();
	virtual ~Mesh();

	virtual bool Load(const std::string& _filePath) = 0;
	
	void AddTextureStage(const std::string& _fileName);
	int CreateTextureStage(PBR_MATERIAL _type, const std::string& _fileName);

	void SetActiveVAO();

	const int& GetTextureID(PBR_MATERIAL _type) { return m_textureStages[_type]; }

	class VertexArray* GetVertexArray() { return m_vertexArray; }

	void CalcTangentVec(glm::vec3& destTangent_,
		                const glm::vec3& _pos1, const glm::vec3& _pos2, const glm::vec3& _pos3,
		                const glm::vec2& _uv1, const glm::vec2& _uv2, const glm::vec2& _uv3);

	void SetTangent(std::vector<float>& destfv_, int _index, const glm::vec3& _tangent);

protected:


	class VertexArray* m_vertexArray;

	std::unordered_map<PBR_MATERIAL, int> m_textureStages;

};