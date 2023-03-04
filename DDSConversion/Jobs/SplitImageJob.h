#pragma once

#include <JobManager/Job.h>

class QImage;
class ImageConversionJobData;

class SplitImageJob
    : public Job
{
public:
    SplitImageJob(std::string sourceImage, std::shared_ptr<ImageConversionJobData>& jobData);
    ~SplitImageJob();

private:
    virtual void Execute() override;

private:
    QImage* m_sourceImage { nullptr };
};
