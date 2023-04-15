#include "BatchJob.h"

void BatchJob::AddJobToBatch(std::shared_ptr<Job> &job)
{
    m_jobs.emplace_back(job);
}

void BatchJob::Execute()
{
    for (const std::shared_ptr<Job>& job : m_jobs)
    {
        job->Start();
    }
}
