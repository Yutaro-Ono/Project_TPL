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
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <string>
#include <unordered_map>
using namespace boost::property_tree;

enum class SECTION_NAME : unsigned char
{
	DISPLAY_HEIGHT,
	DISPLAY_WIDTH,
	DISPLAY_FULLSCREEN,

	GRAPHICS_DEFFERED,
	GRAPHICS_ENVMAP
};

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

	const int& GetScreenSizeW() { return m_displayWidth; }
	const int& GetScreenSizeH() { return m_displayHeight; }

	bool GetEnableDeferred() { return m_graphicsEnableDeferred; }
	bool GetEnableEnvMap() { return m_graphicsEnableEnvMap; }
	bool GetEnableBloom() { return m_graphicsEnableBloom; }

private:

	GameSettings();

	bool LoadIntValue(const ptree& _pt, const std::string& _section, const std::string& _key, int& _val);

	bool LoadBoolValue(const ptree& _pt, const std::string& _section, const std::string& _key, bool& _val);

	// ini�ǂݍ��ݗp�̃v���p�e�B�c���[
	ptree m_pt;

	// ��ʐݒ�p�̕ϐ�
	int m_displayHeight;
	int m_displayWidth;
	bool m_displayFullScreen;

	// �O���t�B�b�N�X�ݒ�
	bool m_graphicsEnableDeferred;
	bool m_graphicsEnableEnvMap;
	bool m_graphicsEnableBloom;

	std::unordered_map<unsigned char, int> m_settingVal;



	friend class GameMain;

};