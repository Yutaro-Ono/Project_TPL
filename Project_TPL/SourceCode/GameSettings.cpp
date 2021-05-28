//----------------------------------------------------------------------------------+
// @file        GameSettings.cpp
// @brief       �Q�[���ݒ�ɗp����l�̓ǂݍ��݁E�����o��
// @note        �V���O���g���p�^�[��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/24   �V�K�쐬
// 2021/ 3/25   �R���p�C���E�����N�����ǉ�
//----------------------------------------------------------------------------------+
#include "GameSettings.h"

// �R���X�g���N�^
GameSettings::GameSettings()
	:m_displayHeight(-1)
	,m_displayWidth(-1)
	,m_displayFullScreen(-1)
	,m_graphicsEnableDeffered(-1)
	,m_graphicsEnableEnvMap(-1)
{

}


// �f�X�g���N�^
GameSettings::~GameSettings()
{
	
}


/// <summary>
/// ini�t�@�C���̓ǂݍ���
/// </summary>
/// <param name="_iniPath"> ini�t�@�C���̑��΃p�X </param>
/// <returns> �ǂݍ��݂ɐ���������true��Ԃ� </returns>
bool GameSettings::Load(const std::string& _iniPath)
{
	// boost�v���p�e�B�c���[��ini�t�@�C����ǂݍ���
	ptree pt;
	read_ini(_iniPath, pt);

	bool loadSuccess = true;

	// ini����f�[�^�����[�h
	loadSuccess = LoadIntValue(pt, "Display", "ScreenWidth", m_displayWidth);
	loadSuccess = LoadIntValue(pt, "Display", "ScreenHeight", m_displayHeight);

	loadSuccess = LoadIntValue(pt, "Display", "FullScreen", m_displayFullScreen);
	loadSuccess = LoadIntValue(pt, "Graphics", "EnableDefferedRendering", m_graphicsEnableDeffered);
	loadSuccess = LoadIntValue(pt, "Graphics", "EnableEnvironmentMap", m_graphicsEnableEnvMap);
	
	return loadSuccess;;
}


/// <summary>
/// ini�t�@�C�����̎w��Z�N�V�����ɂ���w��L�[�̒l(int)�����o��
/// </summary>
/// <param name="_pt"> ini�t�@�C����ǂݍ��񂾃v���p�e�B�c���[ </param>
/// <param name="_section"> ini�t�@�C�����̃Z�N�V������ ��[]�Ɋ���ꂽ������ </param>
/// <param name="_key"> ini�t�@�C�����̃L�[�� ��"  " = �� "" ���̕����� </param>
/// <param name="_val"> �ǂݍ��񂾒l���i�[����ϐ� </param>
/// <returns></returns>
bool GameSettings::LoadIntValue(const ptree& _pt, const std::string& _section, const std::string& _key, int& _val)
{
	std::string sectionKey = _section + "." + _key;
	_val = _pt.get<int>(sectionKey);
	return _val == -1 ? false : true;
}
