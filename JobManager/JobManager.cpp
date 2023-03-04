#include "JobManager.h"

void JobManager::Start()
{
    const uint32_t num_threads = std::thread::hardware_concurrency(); // Max # of threads the system supports
    m_threads.resize(num_threads);
    for (uint32_t i = 0; i < num_threads; i++)
    {
        m_threads.at(i) = std::thread(&JobManager::ThreadLoop, this);
    }
}

void JobManager::QueueJob(const std::shared_ptr<Job> &job)
{
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_jobs.push_back(job);
    }
    //m_mutexCondition.notify_one();
}

void JobManager::Stop()
{
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_bShouldTerminate = true;
    }
    //m_mutexCondition.notify_all();
    for (std::thread& activeThread : m_threads) {
        activeThread.join();
    }
    m_threads.clear();
}

bool JobManager::IsBusy()
{
    bool poolbusy;
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        poolbusy = m_jobs.empty();
    }
    return poolbusy;
}

void JobManager::ThreadLoop()
{
    while (true)
    {
        std::shared_ptr<Job> job = nullptr;
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            job = GetRunnableJob();

            if (!job && m_bShouldTerminate && m_jobs.size() == 0)
            {
                return;
            }
        }

        if (job)
        {
            job->Start();
        }
        else
        {
            std::this_thread::yield();
        }
    }
}

std::shared_ptr<Job> JobManager::GetRunnableJob()
{
    std::shared_ptr<Job> result = nullptr;
    for (std::vector<std::shared_ptr<Job>>::iterator it = m_jobs.begin(); it != m_jobs.end();)
    {
        if ((*it)->IsReady())
        {
            result = *it;
            m_jobs.erase(it);
            break;
        }
        else
        {
            it++;
        }
    }

    return result;
}
