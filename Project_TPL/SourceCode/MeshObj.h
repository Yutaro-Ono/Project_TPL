//----------------------------------------------------------------------------------+
// @file        MeshObj.h
// @brief       ���b�V�����(���_�z��E�e�N�X�`��)��ێ�����
//              
// @note        .obj�`���t�@�C���̓ǂݍ��݂ɑΉ�
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/12   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include "Mesh.h"

class MeshObj : public Mesh
{
public:

	MeshObj();
	~MeshObj();

	bool Load(const std::string& _filePath) override;


};