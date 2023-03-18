#pragma once

#include <JobManager/ThreadConfiguration.h>

#include <vector>
#include <memory>
#include <atomic>

class JobData;

class Job
{
public:
    virtual ~Job() = default;

    void Start();

    void AddJobData(const std::shared_ptr<JobData>& jobData);
    JobData* GetJobData();

    virtual ThreadAffinity GetAffinity();

    void AddDependency(Job* pJob);

    bool IsReady();
    bool IsSuccessful();

protected:
    virtual void Execute() = 0;

private:
    void AddDependentJob(Job* pJob);

    void OnFinish();

private:
    bool m_bSuccessful { false };

    std::atomic_int m_dependeciesCounter { 0 };
    std::vector<Job*> m_dependentJobs;

    std::shared_ptr<JobData> m_jobData;
};
