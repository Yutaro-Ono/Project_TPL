//----------------------------------------------------------------------------------+
// @file        Component.cpp
// @brief       コンポーネントクラス
//              
// @note        ベースクラス
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/14   新規作成
//----------------------------------------------------------------------------------+
#include "Component.h"

int Component::m_globalID = 0;

Component::Component(Actor* _owner, int _updateOrder)
	:m_owner(_owner)
	,m_updateOrder(_updateOrder)
{
	// オーナーアクタにコンポーネントを追加
	m_owner->AddComponent(this);
	// 個別IDの登録とグローバルIDのインクリメント
	m_ID = m_globalID;
	m_globalID++;
}

Component::~Component()
{
	m_owner->RemoveComponent(this);
}
