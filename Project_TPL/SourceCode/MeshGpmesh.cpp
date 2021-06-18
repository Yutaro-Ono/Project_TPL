//----------------------------------------------------------------------------------+
// @file        MeshGpmesh.cpp
// @brief       ���b�V�����(���_�z��E�e�N�X�`��)��ێ�����
//              
// @note        .gpmesh�`���t�@�C���̓ǂݍ��݂ɑΉ�
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/21   �V�K�쐬
//----------------------------------------------------------------------------------+
#include "MeshGpmesh.h"
#include <fstream>
#include <sstream>
#include "VertexArray.h"

MeshGpmesh::MeshGpmesh()
	:m_specular(1.0f)
{
}

MeshGpmesh::~MeshGpmesh()
{
}


/// <summary>
/// gpmesh�t�@�C���̃��[�h
/// </summary>
/// <param name="_filePath"> �ǂݍ���gpmesh�t�@�C���ւ̃p�X </param>
/// <returns> ����ɓǂݍ��߂��ꍇ��true��Ԃ� </returns>
bool MeshGpmesh::Load(const std::string& _filePath)
{
	// gpmesh�t�@�C�����J��
	std::ifstream file(_filePath);
	if (!file.is_open())
	{
		std::cout << "ERROR::GPMESH File Not Found::" << _filePath << std::endl;
		return false;
	}

	// stream�ɊJ����gpmesh�t�@�C������������
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	// �X��string�ɕϊ����Achar�`���Ƃ���rapidjson�̃X�g���[���Ɋi�[����
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStream(contents.c_str());
	// json�h�L�������g�Ƃ��ĉ�͂���
	rapidjson::Document doc;
	doc.ParseStream(jsonStream);

	// json�I�u�W�F�N�g�Ƃ��ĉ�͂ł�����
	if (!doc.IsObject())
	{
		std::cout << "ERROR::Not valid json::" << _filePath << std::endl;
		return false;
	}

	// json�o�[�W�����`�F�b�N
	int ver = doc["version"].GetInt();
	if (ver != 1)
	{
		std::cout << "ERROE::GPMESH is not version 1 ::" << _filePath << std::endl;
		return false;
	}

	//----------------------------------------------------------------------------------------------------+
	// gpmesh�̊e�v�f���擾
	m_shaderName = doc["shader"].GetString();                               // �V�F�[�_�[
	m_specular = static_cast<float>(doc["specularPower"].GetDouble());      // �X�y�L�������x
	std::string vertexFormat = doc["vertexformat"].GetString();             // ���_���C�A�E�g
	//----------------------------------------------------------------------------------------------------+

	//----------------------------------------------------------------------------------------------------+
	// ���_���C�A�E�g��1���_�̏��ʂ��Z�b�g
	VERTEX_LAYOUT::TYPE layout = VERTEX_LAYOUT::TYPE::POS_NORMAL_UV;
	size_t vertexSize = 8;
	if (layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_UV_TAN || layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_SKIN_UV_TAN)
	{
		vertexSize = 11;     // vx, vy, vz, nx, ny, nz, u, v, tx, ty, tz
	}
	//----------------------------------------------------------------------------------------------------+

	//----------------------------------------------------------------------------------------------------+
	// ���_�z��̓ǂݍ���
	// �z�񉻂ł��Ȃ������A�܂��͔z��T�C�Y���m�ۂł��Ȃ������ꍇ�͎��s
	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1)
	{
		std::cout << "ERROR::GPMESH is no vertices::" << _filePath << std::endl;
		return false;
	}
	//----------------------------------------------------------------------------------------------------+

	//----------------------------------------------------------------------------------------------------+
	// ���_�C���f�b�N�X�̓ǂݍ��ݏ���
	const rapidjson::Value& indicesJson = doc["indices"];
	if (!indicesJson.IsArray() || indicesJson.Size() < 1)
	{
		std::cout << "ERROR::GPMESH has no indices::" << _filePath << std::endl;
		return false;
	}
	// ���_�C���f�b�N�X��z��Ɋi�[
	std::vector<unsigned int> indices;
	indices.reserve(indicesJson.Size() * 3);
	for (rapidjson::SizeType i = 0; i < indicesJson.Size(); i++)
	{
		const rapidjson::Value& index = indicesJson[i];
		if (!index.IsArray() || index.Size() != 3)
		{
			std::cout << "ERROR::GPMESH Invalid indices for " << _filePath << std::endl;
			return false;
		}

		indices.emplace_back(index[0].GetUint());
		indices.emplace_back(index[1].GetUint());
		indices.emplace_back(index[2].GetUint());
	}
	//----------------------------------------------------------------------------------------------------+

	//----------------------------------------------------------------------------------------------------+
	// ���_�f�[�^��float�^�z��Ƃ��Ċm�ۂ��Ă���
	// ���_�f�[�^�i�[�p�z��
	std::vector<Vertex> vertices;
	vertices.reserve(vertsJson.Size() * vertexSize);
	// �^���W�F���g��Ԍv�Z�p
	std::vector<glm::vec3> destPos;
	std::vector<glm::vec2> uvPos;
	// m_radius = 0.0f;
	// ���_���ǂݍ��݃��[�v
	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
	{
		// 1�̒��_��񂪔z�񉻂��Ă��邩���`�F�b�N (1�̗v�f�ɕ����̃f�[�^�������Ă��邽��)
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray())
		{
			std::cout << "ERROR::Unexpected vertex format for %s::" << _filePath << std::endl;
			return false;
		}

		//-------------------------------------------------+
		// �����蔻��v�Z

		//-------------------------------------------------+

		//-----------------------------------------------------------------------------+
		// ���_���C�A�E�g���̓ǂݍ��ݏ���
		Vertex v;
		// �{�[���������f���f�[�^
		if (layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_UV || layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_UV_TAN)
		{
			// gpmesh�t�@�C�����璸�_�����m��
			for (rapidjson::SizeType j = 0; j < vert.Size(); j++)
			{
				
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
				
			}

			// ���_���W��UV���W���i�[
			destPos.push_back(glm::vec3(vertices[i * 8 + 0].f, vertices[i * 8 + 1].f, vertices[i * 8 + 2].f));
			uvPos.push_back(glm::vec2(vertices[i * 8 + 6].f, vertices[i * 8 + 7].f));

			// 1�|���S�����̏�񂪏W�܂����璸�_�z��Ɋi�[
			if (destPos.size() == 3)
			{
				// ���_�z��֒ǉ�
				for (int tan = 0; tan < 3; tan++)
				{
					vertices[(i - (2 - tan)) * 8 + 0].f = static_cast<float>(destPos[tan].y);
					vertices[(i - (2 - tan)) * 8 + 1].f = static_cast<float>(destPos[tan].z);
					vertices[(i - (2 - tan)) * 8 + 2].f = static_cast<float>(destPos[tan].x);

					vertices[(i - (2 - tan)) * 8 + 6].f = static_cast<float>(uvPos[tan].x);
					vertices[(i - (2 - tan)) * 8 + 7].f = static_cast<float>(uvPos[tan].y);
				}
				// �ۑ����Ă������_���W�E�e�N�X�`�����W�f�[�^���N���A���Ă���
				uvPos.clear();
				destPos.clear();
			}


			// �@���}�b�v��K�p����ꍇ
			if (layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_UV_TAN)
			{
				// �^���W�F���g�v�Z�p�̒��_���W��UV���W���i�[
				destPos.push_back(glm::vec3(vertices[i * 11 + 0].f, vertices[i * 11 + 1].f, vertices[i * 11 + 2].f));
				uvPos.push_back(glm::vec2(vertices[i * 11 + 6].f, vertices[i * 11 + 7].f));
				// �^���W�F���g��ԗp�̃f�[�^�̈���m��
				for (int tan = 0; tan < 3; tan++)
				{
					v.f = 0.0f;
					vertices.emplace_back(v);
				}

				// 1�|���S�����̏�񂪏W�܂�����^���W�F���g�v�Z���s���A���_�z��ɒǉ�����
				if (destPos.size() == 3)
				{
					glm::vec3 tangent;
					CalcTangentVec(tangent, destPos[0], destPos[1], destPos[2], uvPos[0], uvPos[1], uvPos[2]);
					// ���_�z��֒ǉ�
					for (int tan = 2; tan >= 0; tan--)
					{
						vertices[(i - tan) * 11 + 8 + 0].f = static_cast<float>(tangent.x);
						vertices[(i - tan) * 11 + 8 + 1].f = static_cast<float>(tangent.y);
						vertices[(i - tan) * 11 + 8 + 2].f = static_cast<float>(tangent.z);
					}
					// �ۑ����Ă������_���W�E�e�N�X�`�����W�f�[�^���N���A���Ă���
					uvPos.clear();
					destPos.clear();
				}
			}
		}
		// �{�[�����胂�f���f�[�^
		if (layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_SKIN_UV || layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_SKIN_UV_TAN)
		{
			// ���_���W�Ɩ@����ǉ� vx, vy, vz, nx, ny, nz
			for (rapidjson::SizeType j = 0; j < 6; j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}

			// �X�L�����̒ǉ� (�{�[���ԍ�:unsigned char��1�o�C�g��)
			// 2���[�v (1:�{�[���ԍ�  2:�{�[���E�F�C�g)
			for (rapidjson::SizeType j = 6; j < 14; j += 4)
			{
				v.b[0] = vert[j].GetUint();
				v.b[1] = vert[j + 1].GetUint();
				v.b[2] = vert[j + 2].GetUint();
				v.b[3] = vert[j + 3].GetUint();
				vertices.emplace_back(v);
			}

			// �e�N�X�`�����W�̒ǉ�
			for (rapidjson::SizeType j = 14; j < vert.Size(); j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}

			if (layout == VERTEX_LAYOUT::TYPE::POS_NORMAL_SKIN_UV_TAN)
			{
				// �^���W�F���g�v�Z�p�̒��_���W��UV���W���i�[
				destPos.push_back(glm::vec3(vertices[i * 13 + 0].f, vertices[i * 13 + 1].f, vertices[i * 13 + 2].f));
				uvPos.push_back(glm::vec2(vertices[i * 13 + 8].f, vertices[i * 13 + 9].f));
				// �^���W�F���g��ԗp�̃f�[�^�̈���m��
				for (int tan = 0; tan < 3; tan++)
				{
					v.f = 0.0f;
					vertices.emplace_back(v);
				}

				// 1�|���S�����̏�񂪏W�܂�����^���W�F���g�v�Z���s���A���_�z��ɒǉ�����
				if (destPos.size() == 3)
				{
					glm::vec3 tangent;
					CalcTangentVec(tangent, destPos[0], destPos[1], destPos[2], uvPos[0], uvPos[1], uvPos[2]);
					// ���_�z��֒ǉ�
					for (int tan = 2; tan >= 0; tan--)
					{
						vertices[(i - tan) * 13 + 10 + 0].f = static_cast<float>(tangent.x);
						vertices[(i - tan) * 13 + 10 + 1].f = static_cast<float>(tangent.y);
						vertices[(i - tan) * 13 + 10 + 2].f = static_cast<float>(tangent.z);
					}
					// �ۑ����Ă������_���W�E�e�N�X�`�����W�f�[�^���N���A���Ă���
					uvPos.clear();
					destPos.clear();
				}
			}

		}
		// ���_���C�A�E�g���̓ǂݍ��ݏ��� �����܂�
		//----------------------------------------------------------------------------+

	}

	// �e�N�X�`���X�e�[�W�̒ǉ�
	AddTextureStage(_filePath);

	// ���_�z��N���X�̐���
	m_vertexArray = new VertexArray(vertices.data(), static_cast<unsigned>(vertices.size()) / vertexSize, layout, indices.data(), static_cast<unsigned>(indices.size()));

	return true;
}
