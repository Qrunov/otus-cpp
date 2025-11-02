#pragma once
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <iostream>
#include <list>

using namespace std;
template <class T>
class workQueue
{
public:
	workQueue() {};

	void put(shared_ptr<T> elem)
	{
		lock_guard<mutex> lock(m_queueLock);
		m_queue.push_back(elem);
		m_conditional.notify_one();
	}

	shared_ptr<T> get()
	{
		unique_lock<mutex> lk(m_queueLock);
		m_conditional.wait(lk, [&]
						   { return m_stopped == true || !m_queue.empty(); });
		if (!m_queue.empty())
		{
			auto res = m_queue.front();
			m_queue.pop_front();
			return res;
		}
		else
			return {};
	}

	void stopQueue()
	{
		unique_lock<mutex> lk(m_queueLock);
		m_stopped = true;
		m_conditional.notify_all();
	}

private:
	mutex m_queueLock;
	condition_variable m_conditional;
	list<shared_ptr<T>> m_queue;
	bool m_stopped{false};
};
