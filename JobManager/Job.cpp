#include "Job.h"

#include <JobManager/JobData.h>

#include <algorithm>

void Job::Start()
{
    Execute();

    m_bSuccessful = true;

    OnFinish();
}

void Job::AddJobData(const std::shared_ptr<JobData> &jobData)
{
    m_jobData = jobData;
}

bool Job::IsReady()
{
    return m_dependeciesCounter == 0;
}

bool Job::IsSuccessful()
{
    return m_bSuccessful;
}

void Job::AddDependentJob(Job *pJob)
{
    m_dependentJobs.emplace_back(pJob);
}

void Job::OnFinish()
{
    for (const auto& dependentJob : m_dependentJobs)
    {
        dependentJob->m_dependeciesCounter--;
    }
}

JobData* Job::GetJobData()
{
    return m_jobData.get();
}

ThreadAffinity Job::GetAffinity()
{
    return ThreadAffinity::Default;
}

void Job::AddDependency(Job* pJob)
{
    pJob->AddDependentJob(this);
    m_dependeciesCounter++;
}
