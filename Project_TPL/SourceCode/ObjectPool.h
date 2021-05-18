//----------------------------------------------------------------------------------+
// @file        ObjectPool.h
// @brief       �I�u�W�F�N�g�v�[��
// @note        �x�[�X�N���X
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/7   �V�K�쐬
// 2021/ 5/12  �v�[����A�z�z��ɕύX
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
	
	std::vector<class T*> m_objPool;
	std::unordered_map<std::string, class T*> m_keyObjPool;
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
	// �I�u�W�F�N�g�v�[���������
	while (!m_objPool.empty())
	{
		delete m_objPool.back();
	}


	// �I�u�W�F�N�g�v�[��(unordered_map)�������
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
