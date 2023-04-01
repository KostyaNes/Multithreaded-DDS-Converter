#pragma once

#include <JobManager/Job.h>

class BatchJob
    : public Job
{
public:
    void AddJob(std::shared_ptr<Job>& job);

private:
    virtual void Execute() override;

private:
    std::vector<std::shared_ptr<Job>> m_jobs;
};

