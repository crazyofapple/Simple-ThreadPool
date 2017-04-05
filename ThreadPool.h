#pragma once
#ifdef WIN32
#include <pthread.h>
#include <time.h>
#define random rand
#else
#include <pthread.h>
#endif
#include <list>
using namespace std;

class CThreadNotify {
public:
	CThreadNotify();
	~CThreadNotify();
	void Lock() { pthread_mutex_lock(&m_mutex); }
	void Unlock() { pthread_mutex_unlock(&m_mutex); }
	void Wait() { pthread_cond_wait(&m_cond, &m_mutex); }
	void Signal() { pthread_cond_signal(&m_cond); }
private:
	pthread_mutex_t m_mutex;
	pthread_mutexattr_t m_mutexattr;
	pthread_cond_t m_cond;
};
class CTask {
public:
	CTask() {};
	virtual ~CTask() {};
	virtual void run() = 0;
private:
};
class CWorkerThread {
public:
	CWorkerThread():m_task_cnt(0) {};
	~CWorkerThread() {};

	static void* StartRoutine(void* arg);
	void Start();
	void Execute();
	void PushTask(CTask* pTask);
	void SetThreadIdx(uint32_t idx) { m_thread_idx = idx; }
private:
	uint32_t m_thread_idx;
	uint32_t m_task_cnt;
	pthread_t m_thread_id;
	CThreadNotify m_thread_notify;
	list<CTask*> m_task_list;
};
class ThreadPool
{
public:
	ThreadPool() :m_worker_size(0), m_worker_list(nullptr) {};
	virtual ~ThreadPool() {};
	int Init(uint32_t worker_size);
	void AddTask(CTask* pTask);
private:
	uint32_t m_worker_size;
	CWorkerThread* m_worker_list;
};

