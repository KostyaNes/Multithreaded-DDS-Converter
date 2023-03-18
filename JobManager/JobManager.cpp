#include "JobManager.h"

void JobManager::Start()
{
    const uint32_t num_threads = std::thread::hardware_concurrency(); // Max # of threads the system supports
    m_threads.resize(num_threads);
    for (uint32_t i = 0; i < num_threads; i++)
    {
        m_threads.at(i) = std::thread(&JobManager::ThreadLoop, this, i < m_threadsConfiguration.size() ? m_threadsConfiguration[i] : ThreadConfiguration());
    }
}

void JobManager::QueueJob(const std::shared_ptr<Job> &job)
{
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_jobs[job->GetAffinity()].push_back(job);
        m_jobsCount++;
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

void JobManager::AddThreadConfiguration(const ThreadConfiguration &configuration)
{
    m_threadsConfiguration.push_back(configuration);
}

void JobManager::SetupBasicConfigurationWithIOThread(JobManager &jobManager)
{
    jobManager.AddThreadConfiguration(ThreadConfiguration(ThreadAffinity::IO, /*bStrict*/ false));
}

void JobManager::ThreadLoop(const ThreadConfiguration &configuration)
{
    while (true)
    {
        std::shared_ptr<Job> job = nullptr;
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            job = GetRunnableJob(configuration);

            if (!job && m_bShouldTerminate && m_jobsCount == 0)
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

std::shared_ptr<Job> JobManager::GetRunnableJob(const ThreadConfiguration &configuration)
{
    std::shared_ptr<Job> result = nullptr;
    const auto& affinitiveJobs = m_jobs.find(configuration.GetAffinity());
    if (affinitiveJobs != m_jobs.end())
    {
        for (std::vector<std::shared_ptr<Job>>::iterator it = affinitiveJobs->second.begin(); it != affinitiveJobs->second.end();)
        {
            if ((*it)->IsReady())
            {
                result = *it;
                affinitiveJobs->second.erase(it);
                break;
            }
            else
            {
                it++;
            }
        }
    }

    if (!configuration.IsStrict() && !result)
    {
        for (ThreadAffinity affinity = ThreadAffinity::Default; affinity < ThreadAffinity::Strict; affinity = static_cast<ThreadAffinity>(static_cast<int>(affinity) + 1))
        {
            const auto& affinitiveJobs = m_jobs.find(affinity);
            if (affinitiveJobs != m_jobs.end())
            {
                for (std::vector<std::shared_ptr<Job>>::iterator it = affinitiveJobs->second.begin(); it != affinitiveJobs->second.end();)
                {
                    if ((*it)->IsReady())
                    {
                        result = *it;
                        affinitiveJobs->second.erase(it);
                        break;
                    }
                    else
                    {
                        it++;
                    }
                }
            }
        }
    }

    if (result)
    {
        m_jobsCount--;
    }

    return result;
}
