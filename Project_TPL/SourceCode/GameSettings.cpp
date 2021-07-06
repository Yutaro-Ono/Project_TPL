//----------------------------------------------------------------------------------+
// @file        GameSettings.cpp
// @brief       ゲーム設定に用いる値の読み込み・書き出し
// @note        シングルトンパターン
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/24   新規作成
// 2021/ 3/25   コンパイル・リンク処理追加
//----------------------------------------------------------------------------------+
#include "GameSettings.h"

// コンストラクタ
GameSettings::GameSettings()
	:m_displayHeight(-1)
	,m_displayWidth(-1)
	,m_displayFullScreen(-1)
	,m_graphicsEnableDeferred(-1)
	,m_graphicsEnableEnvMap(-1)
{

}


// デストラクタ
GameSettings::~GameSettings()
{
	
}


/// <summary>
/// iniファイルの読み込み
/// </summary>
/// <param name="_iniPath"> iniファイルの相対パス </param>
/// <returns> 読み込みに成功したらtrueを返す </returns>
bool GameSettings::Load(const std::string& _iniPath)
{
	// boostプロパティツリーにiniファイルを読み込み
	ptree pt;
	read_ini(_iniPath, pt);

	bool loadSuccess = true;

	// iniからデータをロード
	loadSuccess = LoadIntValue(pt, "Display", "ScreenWidth", m_displayWidth);
	loadSuccess = LoadIntValue(pt, "Display", "ScreenHeight", m_displayHeight);

	loadSuccess = LoadBoolValue(pt, "Display", "FullScreen", m_displayFullScreen);
	loadSuccess = LoadBoolValue(pt, "Graphics", "EnableDeferredRendering", m_graphicsEnableDeferred);
	loadSuccess = LoadBoolValue(pt, "Graphics", "EnableEnvironmentMap", m_graphicsEnableEnvMap);
	loadSuccess = LoadBoolValue(pt, "Graphics", "EnableBloom", m_graphicsEnableBloom);

	return loadSuccess;;
}


/// <summary>
/// iniファイル内の指定セクションにある指定キーの値(int)を取り出す
/// </summary>
/// <param name="_pt"> iniファイルを読み込んだプロパティツリー </param>
/// <param name="_section"> iniファイル内のセクション名 ※[]に括られた文字列 </param>
/// <param name="_key"> iniファイル内のキー名 ※"  " = の "" 内の文字列 </param>
/// <param name="_val"> 読み込んだ値を格納する変数 </param>
/// <returns></returns>
bool GameSettings::LoadIntValue(const ptree& _pt, const std::string& _section, const std::string& _key, int& _val)
{
	std::string sectionKey = _section + "." + _key;
	_val = _pt.get<int>(sectionKey);
	return _val == -1 ? false : true;
}

bool GameSettings::LoadBoolValue(const ptree& _pt, const std::string& _section, const std::string& _key, bool& _val)
{
	std::string sectionKey = _section + "." + _key;
	_val = _pt.get<bool>(sectionKey);
	return _val == -1 ? false : true;
}
