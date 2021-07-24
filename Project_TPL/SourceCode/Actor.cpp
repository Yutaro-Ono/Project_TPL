//----------------------------------------------------------------------------------+
// @file        Actor.cpp
// @brief       アクタークラス (座標を持ち、描画されるオブジェクト全般の基底クラス)
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/23   新規作成
//----------------------------------------------------------------------------------+
#include "Actor.h"
#include "GameMain.h"
#include "Debugger.h"
#include "ActorPool.h"
#include "ActorDebugObject.h"

int Actor::m_globalActorNo = 0;

Actor::Actor(OBJECT_TAG _tag)
	:m_state(ActorEnum::ACTOR_STATE::ACTIVE)
	,m_position(Vector3::Zero)
	,m_scale(Vector3(1.0f, 1.0f, 1.0f))
	,m_rotation(Quaternion::Identity)
	,m_recomputeWorldTransform(true)
	,m_ID(m_globalActorNo)
	,m_debugObj(nullptr)
{
	// アクタープールに追加する
	ACTOR_POOL->AddObject(this);

	// ゲーム全体のアクター番号をインクリメント
	m_globalActorNo++;

	ComputeWorldTransform();

	// デバッグオブジェクトの生成
#ifdef _DEBUG

	m_debugObj = new ActorDebugObject(this);
	DEBUGGER->AddDebugObject(m_debugObj, _tag);

#endif
}

Actor::~Actor()
{

	printf("DELETE::Actor ID = %d\n", m_ID);

	// コンポーネントの削除
	while (!m_components.empty())
	{
		delete m_components.back();
	}

}

void Actor::Update(float _deltaTime)
{
	// アクターがアクティブなら
	if (m_state == ActorEnum::ACTOR_STATE::ACTIVE)
	{
		// ワールド変換行列を計算
		ComputeWorldTransform();

		// コンポーネントの更新処理
		UpdateComponents(_deltaTime);
		// アクター固有の更新処理
		UpdateActor(_deltaTime);

		// ワールド変換行列の再計算
		ComputeWorldTransform();
	}
}

/// <summary>
/// コンポーネントの更新処理
/// </summary>
/// <param name="_deltaTime"></param>
void Actor::UpdateComponents(float _deltaTime)
{
}

/// <summary>
/// アクター固有の更新処理
/// </summary>
/// <param name="_deltaTime"></param>
void Actor::UpdateActor(float _deltaTime)
{
	
}

/// <summary>
/// ワールド変換行列の更新処理
/// </summary>
void Actor::ComputeWorldTransform()
{
	// ワールド変換行列の再計算が必要な場合のみ実行
	if (m_recomputeWorldTransform)
	{
		// スケーリング→回転→平行移動となるように変換行列を作成
		m_worldTrans = Matrix4::CreateScale(m_scale);
		m_worldTrans *= Matrix4::CreateFromQuaternion(m_rotation);
		m_worldTrans *= Matrix4::CreateTranslation(m_position);

		// アクターが持っている全コンポーネントの変換を行う
		for (auto comp : m_components)
		{
			comp->OnUpdateTransform();
		}

		m_recomputeWorldTransform = false;
	}
}

/// <summary>
/// コンポーネントの追加処理
/// </summary>
/// <param name="_comp"> 追加するコンポーネント </param>
void Actor::AddComponent(Component* _comp)
{
	// コンポーネントをソートして追加
	// 自分のオーダーより大きい挿入点を検索
	int myOrder = _comp->GetUpdateOrder();
	auto iter = m_components.begin();

	for (; iter != m_components.end(); ++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}

	// 要素を見つけた手前に挿入する
	m_components.insert(iter, _comp);
}

/// <summary>
/// コンポーネントの削除処理
/// </summary>
/// <param name="_comp"> 削除したいコンポーネント </param>
void Actor::RemoveComponent(Component* _comp)
{
	// 渡されたコンポーネントを検索し、削除する
	auto iter = std::find(m_components.begin(), m_components.end(), _comp);
	if (iter != m_components.end())
	{
		m_components.erase(iter);
	}
}

/// <summary>
/// 座標のセット
/// ワールド変換行列の再計算のフラグを立てる
/// </summary>
/// <param name="_pos"></param>
void Actor::SetPosition(const Vector3& _pos)
{
	m_position = _pos;
	m_recomputeWorldTransform = true;
}

/// <summary>
/// スケールのセット
/// ワールド変換行列の再計算のフラグを立てる
/// </summary>
/// <param name="_scale"></param>
void Actor::SetScale(const Vector3& _scale)
{
	m_scale = _scale;
	m_recomputeWorldTransform = true;
}

void Actor::RotateToNewForward(const Vector3& in_forward)
{
	// X軸ベクトル(1, 0, 0)とforward間の角度を求める
	float dot = Vector3::Dot(Vector3::UnitX, in_forward);
	float angle = Math::Acos(dot);

	// 下向きだった場合
	if (dot > 0.9999f)
	{
		SetRotation(Quaternion::Identity);
	}
	// 上向きだった場合
	else if (dot < -0.9999f)
	{
		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	}
	else
	{
		// 軸ベクトルとforwardとの外積から回転軸を求めて回転させる
		Vector3 axis = Vector3::Cross(Vector3::UnitX, in_forward);
		axis.Normalize();
		SetRotation(Quaternion(axis, angle));
	}
}

void Actor::SetRotation(const Quaternion& _rot)
{
	m_rotation = _rot;
	m_recomputeWorldTransform = true;
}

