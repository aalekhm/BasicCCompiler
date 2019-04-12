#pragma once

template <typename T>
struct AutoLister
{
	public:
		AutoLister()
		{
			m_pNext = Head();
			Head() = static_cast<T*>(this);
		};

		T* Next() { return m_pNext; }
		const T* Next() const { return m_pNext; }

		static T*& Head() { static T* p = 0; return p; }
	private:
		T*		m_pNext;
};