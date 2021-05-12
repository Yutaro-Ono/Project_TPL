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
#include <list>
#include <unordered_map>
#include <string>

template <class T> class ObjectPool
{

public:

	ObjectPool() {};
	virtual ~ObjectPool() {};

	void Create(int _size);
	void Delete();

	void AddObject(const std::string& _key, T* _obj);
	void DeleteObject(const std::string& _key);

protected:
	
	std::unordered_map<std::string, T*> m_objPool;
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
	for (auto itr : m_objPool)
	{
		printf("RELEASE::Object %s\n", itr.first.c_str());
		delete itr.second;
	}

	m_objPool.clear();
}

template<class T>
inline void ObjectPool<T>::AddObject(const std::string& _key, T* _obj)
{
	auto itr = m_objPool.find(_key);

	if (itr != m_objPool.end())
	{
		return;
	}
	else
	{
		m_objPool.emplace(_key, _obj);
	}
}

template<class T>
inline void ObjectPool<T>::DeleteObject(const std::string& _key)
{
	auto itr = m_objPool.find(_key);

	if (itr != m_objPool.end())
	{
		printf("DELETE::Object %s\n", _key.c_str());
		delete itr->second;
		m_objPool.erase(itr);
	}
	else
	{
		printf("UNDELETE::Not Found Object %s\n", _key.c_str());
	}
}
