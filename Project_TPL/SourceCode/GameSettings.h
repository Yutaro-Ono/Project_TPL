//----------------------------------------------------------------------------------+
// @file        GameSettings.h
// @brief       ゲーム設定に用いる値の読み込み・書き出し
// @note        シングルトンパターン
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/24   新規作成
// 2021/ 3/25   コンパイル・リンク処理追加
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

	// ini読み込み用のプロパティツリー
	ptree m_pt;

	// 画面設定用の変数
	int m_displayHeight;
	int m_displayWidth;
	bool m_displayFullScreen;

	// グラフィックス設定
	bool m_graphicsEnableDeferred;
	bool m_graphicsEnableEnvMap;
	bool m_graphicsEnableBloom;

	std::unordered_map<unsigned char, int> m_settingVal;



	friend class GameMain;

};