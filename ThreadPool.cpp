#include "ThreadPool.h"
#include <cstdlib>


CThreadNotify::CThreadNotify()
{
	pthread_mutexattr_init(&m_mutexattr);
	pthread_mutexattr_settype(&m_mutexattr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&m_mutex, &m_mutexattr);
	pthread_cond_init(&m_cond, NULL);
}

CThreadNotify::~CThreadNotify()
{
	pthread_mutexattr_destroy(&m_mutexattr);
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_cond);
}
//////////////////////////
void * CWorkerThread::StartRoutine(void * arg)
{
	CWorkerThread* pThread = (CWorkerThread*)arg;
	pThread->Execute();
	return nullptr;
}

void CWorkerThread::Start()
{
	(void)pthread_create(&m_thread_id, nullptr, StartRoutine, this);
}

void CWorkerThread::Execute()
{
	while (true) {
		m_thread_notify.Lock();
		while (m_task_list.empty()) {
			m_thread_notify.Wait();
		}
		CTask* pTask = m_task_list.front();
		m_task_list.pop_front();
		m_thread_notify.Unlock();
		pTask->run();
		delete pTask;
		m_task_cnt++;
	}
}

void CWorkerThread::PushTask(CTask * pTask)
{
	m_thread_notify.Lock();
	m_task_list.push_back(pTask);
	m_thread_notify.Signal();
	m_thread_notify.Unlock();
}

//////////////////////////

int ThreadPool::Init(uint32_t worker_size)
{
	m_worker_size = worker_size;
	m_worker_list = new CWorkerThread[m_worker_size];
	if (!m_worker_list) return 1;

	for (uint32_t i = 0; i < m_worker_size; i++) {
		m_worker_list[i].SetThreadIdx(i);
		m_worker_list[i].Start();
	}
	return 0;
}

void ThreadPool::AddTask(CTask * pTask)
{
	uint32_t thread_idx = random() % m_worker_size;
	m_worker_list[thread_idx].PushTask(pTask);
}



