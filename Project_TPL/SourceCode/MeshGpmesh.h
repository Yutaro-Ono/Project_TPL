//----------------------------------------------------------------------------------+
// @file        MeshGpmesh.h
// @brief       ���b�V�����(���_�z��E�e�N�X�`��)��ێ�����
//              
// @note        .gpmesh�`���t�@�C���̓ǂݍ��݂ɑΉ�
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/21   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "Mesh.h"
#include <document.h>


class MeshGpmesh : public Mesh
{

public:

	MeshGpmesh();
	~MeshGpmesh();

	bool Load(const std::string& _filePath) override;


private:



	float m_specular;
	std::string m_shaderName;

};