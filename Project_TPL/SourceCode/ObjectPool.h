//----------------------------------------------------------------------------------+
// @file        ObjectPool.h
// @brief       オブジェクトプール
// @note        ベースクラス
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/7   新規作成
// 2021/ 5/12  プールを連想配列に変更
//----------------------------------------------------------------------------------+
#pragma once
#include <vector>
#include <unordered_map>
#include <string>

template <class T> class ObjectPool
{

public:

	ObjectPool() {};
	virtual ~ObjectPool() {};

	void Create(int _size);
	void Delete();

	void AddObject(T* _obj);
	void DeleteObject(T* _obj);

	void AddKeyObject(const std::string& _key, T* _obj);
	void DeleteKeyObject(const std::string& _key);

	virtual void UpdateObjects(float _deltaTime) = 0;


protected:
	
	std::vector<T*> m_objPool;
	std::unordered_map<std::string, T*> m_keyObjPool;
};

template<class T>
inline void ObjectPool<T>::Create(int _size)
{
	for (int i = 0; i < _size; i++)
	{
		T* t = new T();
		m_objPool.push_back(t);
	}
}

template<class T>
inline void ObjectPool<T>::Delete()
{
	// オブジェクトプール解放処理
	while (!m_objPool.empty())
	{
		delete m_objPool.back();
		m_objPool.pop_back();
	}

	// オブジェクトプール(unordered_map)解放処理
	for (auto itr : m_keyObjPool)
	{
		printf("RELEASE::Object Key = %s\n", itr.first.c_str());
		delete itr.second;
	}
	m_keyObjPool.clear();
}

template<class T>
inline void ObjectPool<T>::AddObject(T* _obj)
{
	m_objPool.emplace_back(_obj);
}

template<class T>
inline void ObjectPool<T>::DeleteObject(T* _obj)
{
	auto iter = std::find(m_objPool.begin(), m_objPool.end(), _obj);

	if (iter != m_objPool.end())
	{
		std::iter_swap(iter, m_objPool.end() - 1);
		m_objPool.pop_back();
	}
}

template<class T>
inline void ObjectPool<T>::AddKeyObject(const std::string& _key, T* _obj)
{
	auto itr = m_keyObjPool.find(_key);

	if (itr != m_keyObjPool.end())
	{
		return;
	}
	else
	{
		m_keyObjPool.emplace(_key, _obj);
	}
}

template<class T>
inline void ObjectPool<T>::DeleteKeyObject(const std::string& _key)
{
	auto itr = m_keyObjPool.find(_key);

	if (itr != m_keyObjPool.end())
	{
		printf("DELETE::Object %s\n", _key.c_str());
		delete itr->second;
		m_keyObjPool.erase(itr);
	}
	else
	{
		printf("UNDELETE::Not Found Object %s\n", _key.c_str());
	}
}
