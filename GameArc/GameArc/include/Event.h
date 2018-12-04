#pragma once
///http://geekswithblogs.net/raccoon_tim/archive/2011/09/28/lambdas-and-events-in-c.aspx
#include <vector>
#include <functional>

template<typename T1>
class Event
{
public:
	typedef std::function<void(T1)> Func;

	Event() {};
	~Event() {};

	void Call(T1 arg)
	{
		for (auto i = m_handlers.begin(); i != m_handlers.end(); i++)
		{
			(*i)(arg);
		}
	}

	void Call()
	{
		for (auto i = m_handlers.begin(); i != m_handlers.end(); i++)
		{
			(*i)();
		}
	}

	void operator ()(T1 arg)
	{
		Call(arg);
	}

	void operator ()()
	{
		Call();
	}

	Event& registerFunction(Func f) {
		m_handlers.push_back(f);
		return *this;
	}

	Event& operator += (Func f)
	{
		m_handlers.push_back(f);
		return *this;
	}

	Event& operator -= (Func f)
	{
		for (auto i = m_handlers.begin(); i != m_handlers.end(); i++)
		{
			if ((*i).target<void(T1)>() == f.target<void(T1)>())
			{
				m_handlers.erase(i);
				break;
			}
		}

		return *this;
	}
private:
	vector<Func> m_handlers;
};