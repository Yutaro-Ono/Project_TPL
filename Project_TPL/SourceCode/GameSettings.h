//----------------------------------------------------------------------------------+
// @file        GameSettings.h
// @brief       �Q�[���ݒ�ɗp����l�̓ǂݍ��݁E�����o��
// @note        �V���O���g���p�^�[��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/24   �V�K�쐬
// 2021/ 3/25   �R���p�C���E�����N�����ǉ�
//----------------------------------------------------------------------------------+
#pragma once
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
using namespace boost::property_tree;



class GameSettings
{
public:


	~GameSettings();

	static GameSettings& GetInstance()
	{
		static GameSettings settings;
		return settings;
	}


	bool Load(const std::string& _iniPath);


private:

	GameSettings();

	bool LoadIntValue(const ptree& _pt, const std::string& _section, const std::string& _key, int& _val);


	// ini�ǂݍ��ݗp�̃v���p�e�B�c���[
	ptree m_pt;

	// ��ʐݒ�p�̕ϐ�
	int m_displayHeight;
	int m_displayWidth;
	int m_displayFullScreen;

	// �O���t�B�b�N�X�ݒ�
	int m_graphicsEnableDeffered;
	int m_graphicsEnableEnvMap;

	friend class GameMain;

};