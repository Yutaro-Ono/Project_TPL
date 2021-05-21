//----------------------------------------------------------------------------------+
// @file        MeshObj.cpp
// @brief       ���b�V�����(���_�z��E�e�N�X�`��)��ێ�����
//              
// @note        .obj�`���t�@�C���̓ǂݍ��݂ɑΉ�
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/12   �V�K�쐬
// 2021/ 5/14   ���b�V���̓ǂݍ��ݏ���������
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
	VERTEX_LAYOUT::TYPE layout = VERTEX_LAYOUT::TYPE::POS_NORMAL_UV;

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


	// �X�g���C�h�̒�`
	int attribStride = attribVertNum + attribNormNum + attribUVNum;
	// �^���W�F���g��񂪕K�v�ȏꍇ�̓X�g���C�h���g��
	if (layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_UV_TAN || layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_SKIN_UV_TAN)
	{
		// �^���W�F���g��� (�m�[�}���}�b�v�Ŏg�p)
		const int attribTanNum = 3;

		attribStride += attribTanNum;
	}


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
	glm::vec3 tangent;
	// ���_���W�E�e�N�X�`��UV�i�[�p
	std::vector<glm::vec3> destPos(3);
	std::vector<glm::vec2> uvPos(3);

	// �`�󃋁[�v
	for (const auto& shape : shapes)
	{
		size_t indexOffset = 0;

		// �ʐ����[�v
		for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
		{
			size_t num_vertices = shape.mesh.num_face_vertices[f];

			// ���_�����[�v (1�ʂ�3��)
			for (size_t v = 0; v < num_vertices; v++)
			{

				// �ʂ��\������C���f�b�N�X���擾
				tinyobj::index_t idx = shape.mesh.indices[indexOffset + v];

				// ���_���W�𒸓_�z��ɃR�s�[
				vertexVec[idx.vertex_index * attribStride + 0] = attrib.vertices[3 * idx.vertex_index + 1];
				vertexVec[idx.vertex_index * attribStride + 1] = attrib.vertices[3 * idx.vertex_index + 0];
				vertexVec[idx.vertex_index * attribStride + 2] = attrib.vertices[3 * idx.vertex_index + 2];

				// �@���f�[�^�𒸓_�z��ɃR�s�[
				vertexVec[idx.vertex_index * attribStride + 3] = attrib.vertices[3 * idx.normal_index + 0];
				vertexVec[idx.vertex_index * attribStride + 4] = attrib.vertices[3 * idx.normal_index + 1];
				vertexVec[idx.vertex_index * attribStride + 5] = attrib.vertices[3 * idx.normal_index + 2];

				// uv�f�[�^�𒸓_�z��ɃR�s�[
				vertexVec[idx.vertex_index * attribStride + 6] = attrib.texcoords[2 * idx.texcoord_index + 0];
				vertexVec[idx.vertex_index * attribStride + 7] = 1.0f - attrib.texcoords[2 * idx.texcoord_index + 1];


				//-------------------------------------------------------+
				// �����蔻��{�b�N�X�E���a�̎Z�o
				//-------------------------------------------------------+
				// 



				// �|���S�����\�����钸�_���W���ꎞ�ۑ�
				destPos[v] = glm::vec3(vertexVec[(int)idx.vertex_index * attribStride + 0],
					                   vertexVec[(int)idx.vertex_index * attribStride + 1],
					                   vertexVec[(int)idx.vertex_index * attribStride + 2]);

				// �e�N�X�`�����W���ꎞ�ۑ�
				uvPos[v] = glm::vec2(vertexVec[(int)idx.vertex_index * attribStride + 6],
					                 vertexVec[(int)idx.vertex_index * attribStride + 7]);

			}

			// �^���W�F���g��ԏ�񂪕K�v�ȃ��f���̂�
			if (layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_UV_TAN || layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_SKIN_UV_TAN)
			{
				// �^���W�F���g�̎Z�o
				CalcTangentVec(tangent, destPos[0], destPos[1], destPos[2], uvPos[0], uvPos[1], uvPos[2]);
				// �^���W�F���g���𒸓_�z��Ɋi�[
				for (int i = 0; i < 3; i++)
				{
					SetTangent(vertexVec, indexOffset + i, tangent);
				}
			}


			// ��C���f�b�N�X��ύX
			indexOffset += num_vertices;

		}
	}

	// �ǂݍ��񂾒��_�z�����GL�ɓo�^����
	m_vertexArray = new VertexArray( static_cast<float*>(vertexVec.data()), static_cast<unsigned>(vertNum),
		                             layout, indices.data(), static_cast<unsigned>(indices.size()) );

	// �e�N�X�`���̒ǉ�
	AddTextureStage(_filePath);


	return true;
}
