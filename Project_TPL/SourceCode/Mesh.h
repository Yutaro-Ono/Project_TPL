//----------------------------------------------------------------------------------+
// @file        Mesh.h
// @brief       ���b�V�����(���_�z��E�e�N�X�`��)��ێ�����
//              
// @note        �e�N���X (�q�N���X�̓t�@�C���`�����ɒ�`)
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/7   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include "rapidjson/document.h"
#include "tinyObj/tiny_obj_loader.h"

// �e�N�X�`���̎�� (PBR�}�e���A��)
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