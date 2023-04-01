#pragma once

#include <JobManager/Job.h>

class ImageConversionJobData;

class ConvertImageJob
    : public Job
{
public:
    ConvertImageJob(std::shared_ptr<ImageConversionJobData>& jobData);
    ConvertImageJob(std::shared_ptr<ImageConversionJobData>& jobData, int texelBegin, int texelEnd);

private:
    virtual void Execute() override;

private:
    int m_texelBegin { 0 };
    int m_texelEnd { 0 };
};

