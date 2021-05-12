//----------------------------------------------------------------------------------+
// @file        Mesh.cpp
// @brief       ���b�V�����(���_�z��E�e�N�X�`��)��ێ�����
//              
// @note        �e�N���X (�q�N���X�̓t�@�C���`�����ɒ�`)
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/7   �V�K�쐬
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
/// �e�N�X�`���X�e�[�W�̒ǉ����� (�e��e�N�X�`��1����)
/// ���b�V���̃t�@�C��������g���q���������� �� building.obj �� building
/// </summary>
/// <param name="_fileName"> ���b�V���t�@�C���̖��O </param>
void Mesh::AddTextureStage(const std::string& _fileName)
{
	// �t�@�C���g���q�O�� "." ���������ڂɂ��邩���擾����
	int extNum = _fileName.find_last_of(".");
	// �g���q���������t�@�C�����݂̂��擾
	std::string path = _fileName.substr(0, extNum);

	// �e��e�N�X�`���̎擾 (1����)
	m_textureStages.emplace(PBR_MATERIAL::ALBEDO, CreateTextureStage(PBR_MATERIAL::ALBEDO, path));
	m_textureStages.emplace(PBR_MATERIAL::NORMAL, CreateTextureStage(PBR_MATERIAL::NORMAL, path));
	m_textureStages.emplace(PBR_MATERIAL::METALLIC, CreateTextureStage(PBR_MATERIAL::METALLIC, path));
	m_textureStages.emplace(PBR_MATERIAL::ROUGHNESS, CreateTextureStage(PBR_MATERIAL::ROUGHNESS, path));
	m_textureStages.emplace(PBR_MATERIAL::AO, CreateTextureStage(PBR_MATERIAL::AO, path));

}

/// <summary>
/// �e�N�X�`���X�e�[�W�̓o�^����
/// </summary>
/// <param name="_type"> PBR�}�e���A���̎�� </param>
/// <param name="_filePath"> �t�@�C���̖��O </param>
/// <returns> �e�N�X�`��ID </returns>
int Mesh::CreateTextureStage(PBR_MATERIAL _type, const std::string& _fileName)
{
	// �}�e���A���^�C�v���Ƃ̃e�N�X�`���p�X���쐬
	std::string texturePath = _fileName + PBR_MATERIAL_NAME[static_cast<int>(_type)];

	// �e�N�X�`���̐���
	Texture* t = nullptr;
	t->LoadTexture(texturePath);

	// �e�N�X�`���̎擾�ɐ���������ID��Ԃ�
	if (t != nullptr)
	{
		return t->GetTextureID();
	}


	return 0;
}
