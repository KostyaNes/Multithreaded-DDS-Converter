#pragma once

#include <JobManager/Job.h>

class ImageConversionJobData;

class ConvertImageJob
    : public Job
{
public:
    ConvertImageJob(std::shared_ptr<ImageConversionJobData>& jobData);

private:
    virtual void Execute() override;
};

