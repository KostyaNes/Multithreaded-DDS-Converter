#pragma once

#include "Job.h"

#include <mutex>
#include <vector>
#include <condition_variable>
#include <queue>
#include <thread>
#include <memory>

class JobManager
{
public:
    void Start();
    void QueueJob(const std::shared_ptr<Job>& job);
    void Stop();
    bool IsBusy();

private:
    void ThreadLoop();
    std::shared_ptr<Job> GetRunnableJob();

private:
    bool m_bShouldTerminate { false };           // Tells threads to stop looking for jobs
    std::mutex m_queueMutex;                  // Prevents data races to the job queue
    std::condition_variable m_mutexCondition; // Allows threads to wait on new jobs or termination
    std::vector<std::thread> m_threads;
    std::vector<std::shared_ptr<Job>> m_jobs;
};
