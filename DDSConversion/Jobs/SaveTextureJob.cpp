#include "SaveTextureJob.h"

SaveTextureJob::SaveTextureJob(std::shared_ptr<ImageConversionJobData> &jobData)
{
    AddJobData(jobData);
}

ThreadAffinity SaveTextureJob::GetAffinity()
{
    return ThreadAffinity::IO;
}

void SaveTextureJob::Execute()
{
    assertm(GetJobData(), "ImageConversionJobData has to be set prior to job execution");
    ImageConversionJobData* jobData = reinterpret_cast<ImageConversionJobData*>(GetJobData());

    std::ofstream outputImage;
    outputImage.open(jobData->m_outputFile, std::ios::out | std::ios::binary);

    jobData->m_outputData->Serialize(outputImage);
}
