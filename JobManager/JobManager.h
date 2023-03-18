#pragma once

#include <JobManager/Job.h>

#include <mutex>
#include <vector>
#include <condition_variable>
#include <queue>
#include <thread>
#include <memory>
#include <map>

class JobManager
{
public:
    void Start();
    void QueueJob(const std::shared_ptr<Job>& job);
    void Stop();
    bool IsBusy();

    void AddThreadConfiguration(const ThreadConfiguration& configuration);

    static void SetupBasicConfigurationWithIOThread(JobManager& jobManager);

private:
    void ThreadLoop(const ThreadConfiguration& configuration);
    std::shared_ptr<Job> GetRunnableJob(const ThreadConfiguration &configuration);

private:
    bool m_bShouldTerminate { false };           // Tells threads to stop looking for jobs

    std::mutex m_queueMutex;                  // Prevents data races to the job queue
    std::condition_variable m_mutexCondition; // Allows threads to wait on new jobs or termination

    std::vector<std::thread> m_threads;
    std::vector<ThreadConfiguration> m_threadsConfiguration;

    std::map<ThreadAffinity, std::vector<std::shared_ptr<Job>>> m_jobs;
    int m_jobsCount { 0 }; // Faster to store job count than itterate over map; Thread safe because used only inside of critical sections
};
