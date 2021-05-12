//----------------------------------------------------------------------------------+
// @file        MeshObj.cpp
// @brief       ���b�V�����(���_�z��E�e�N�X�`��)��ێ�����
//              
// @note        .obj�`���t�@�C���̓ǂݍ��݂ɑΉ�
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/12   �V�K�쐬
//----------------------------------------------------------------------------------+
#include "MeshObj.h"
#include "VertexArray.h"

MeshObj::MeshObj()
{
}

MeshObj::~MeshObj()
{
}

/// <summary>
/// .obj�t�@�C���̃��[�h
/// </summary>
/// <param name="_filePath"> ���b�V���t�@�C���p�X </param>
/// <returns> ���Ȃ����[�h�ł����� </returns>
bool MeshObj::Load(const std::string& _filePath)
{
	Vertex::LAYOUT_TYPE layout = Vertex::LAYOUT_TYPE::POS_NORMAL_UV;

	tinyobj::attrib_t attrib;                      // ���W�R���e�i
	std::vector<tinyobj::shape_t> shapes;          // �`��R���e�i
	std::vector<tinyobj::material_t> materials;    // �}�e���A���R���e�i

	// obj�t�@�C���̃��[�h
	std::string warning, error;
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, _filePath.c_str()))
	{
		std::cout << "ERROR::Load Obj Mesh::" << error << std::endl;
		return false;
	}
	
	// �e�풸�_���̌�
	const int attribVertNum = 3;
	const int attribNormNum = 3;
	const int attribUVNum = 2;
	// �^���W�F���g��� (�m�[�}���}�b�v�Ŏg�p)
	const int attribTanNum = 3;

	// �X�g���C�h�̒�`
	const int attribStride = attribVertNum + attribNormNum + attribUVNum + attribTanNum;

	// ���_���̌v�Z (���_���W�̍��v / �|���S���̒��_��(3))
	int vertNum = attrib.vertices.size() / attribVertNum;

	// �C���f�b�N�X�o�b�t�@�̎擾
	std::vector<unsigned int> indices;
	// �`��Ԃ񃋁[�v
	for (const auto& shape : shapes)
	{
		// �C���f�b�N�X�̎擾
		for (const auto& index : shape.mesh.indices)
		{
			indices.push_back(index.vertex_index);
		}
	}

	// ���_�f�[�^�̔z����m�� (���_�f�[�^�� * �X�g���C�h��)
	std::vector<float> vertexVec(vertNum * attribStride);

	// �^���W�F���g��Ԍv�Z�p
	


	return true;
}
