#pragma once

template<typename T>
class Singleton<T>
{
public:
	static T& getInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new T();
		}

		return *m_pInstance;
	}
protected:
private:
	Singleton()
	{ };

	static T*	m_pInstance = nullptr;
};

template<typename T>
T* Singleton<T>::m_pInstance = nullptr;