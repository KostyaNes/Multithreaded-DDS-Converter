#pragma once

#include <JobManager/Job.h>
#include <DDSConversion/Jobs/ImageConversionJobData.h>

class SaveTextureJob
    : public Job
{
public:
    SaveTextureJob(std::shared_ptr<ImageConversionJobData>& jobData);

    virtual ThreadAffinity GetAffinity() override;

private:
    virtual void Execute() override;
};

