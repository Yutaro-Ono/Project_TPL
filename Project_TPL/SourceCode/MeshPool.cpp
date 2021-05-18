//----------------------------------------------------------------------------------+
// @file        MeshPool.cpp
// @brief       ���b�V���v�[�� (�e�N�X�`���N���X���Ǘ�����)
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/12   �V�K�쐬
//----------------------------------------------------------------------------------+
#include "MeshPool.h"
#include "MeshObj.h"
//#include "MeshGpmesh.h"

MeshPool::MeshPool()
{
}

MeshPool::~MeshPool()
{
	Delete();
}

void MeshPool::UpdateObjects(float _deltaTime)
{
}

/// <summary>
/// ���b�V���̎擾�֐�
/// </summary>
/// <param name="_meshPath"></param>
/// <returns> ���b�V���N���X </returns>
Mesh* MeshPool::GetMesh(const std::string& _meshPath)
{
	Mesh* mesh = nullptr;
	auto iter = m_keyObjPool.find(_meshPath);
	
	if (iter != m_keyObjPool.end())
	{
		return iter->second;
	}
	else
	{
		// �t�@�C���̊g���q����A�K�������b�V���N���X�𐶐�����
		// �g���q"."���Ōォ�牽�����ڂɂ��邩�𒲂ׂ� �� �g���q�������擾
		int extNum = _meshPath.find_last_of(".");
		std::string extension = _meshPath.substr(extNum, _meshPath.size() - extNum);

		// .OBJ�`��
		if (extension == ".obj" || extension == ".OBJ")
		{
			mesh = new MeshObj();
			
			// ���b�V���̃��[�h
			if (mesh->Load(_meshPath))
			{
				// ����
				m_keyObjPool.emplace(_meshPath, mesh);
			}
			else
			{
				// ���s
				delete mesh;
				mesh = nullptr;
			}
		}

		// .gpmesh�`��
		if (extension == ".gpmesh")
		{
			
		}
	}


	return mesh;
}
