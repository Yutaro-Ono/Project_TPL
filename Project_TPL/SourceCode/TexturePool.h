//----------------------------------------------------------------------------------+
// @file        TexturePool.h
// @brief       �e�N�X�`���v�[�� (�e�N�X�`���N���X���Ǘ�����)
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/7   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include <string>
#include <unordered_map>
#include "ObjectPool.h"
#include "Texture.h"


class TexturePool : public ObjectPool<Texture>
{

public:

	TexturePool();
	~TexturePool();

	void UpdateObjects(float _deltaTime) override;
	
	class Texture* GetTexture(const std::string& _texturePath);

private:





};