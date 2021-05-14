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

/// <summary>
/// �^���W�F���g�x�N�g���v�Z
/// </summary>
/// <param name="_destTangent"></param>
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
/// ���_�z��Ƀ^���W�F���g������������
/// </summary>
/// <param name="_destfv"> �^���W�F���g�����i�[����z�� </param>
/// <param name="index"> �z��̏������ݐ�C���f�b�N�X </param>
/// <param name="_tangent"> �z��ɏ������ރ^���W�F���g��� </param>
void Mesh::SetTangent(std::vector<float>& destfv_, int _index, const glm::vec3& _tangent)
{
	if (destfv_.size() > _index * 11 + 8 + 2)
	{
		destfv_[_index * 11 + 8 + 0] = _tangent.x;
		destfv_[_index * 11 + 8 + 1] = _tangent.y;
		destfv_[_index * 11 + 8 + 2] = _tangent.z;
	}
}
