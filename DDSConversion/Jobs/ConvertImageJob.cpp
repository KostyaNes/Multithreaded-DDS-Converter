#include "ConvertImageJob.h"

#include <ProjectDefines.h>

#include <DDSConversion/Jobs/ImageConversionJobData.h>

ConvertImageJob::ConvertImageJob(std::shared_ptr<ImageConversionJobData> &jobData)
{
    AddJobData(jobData);
}

void ConvertImageJob::Execute()
{
    assertm(GetJobData(), "ImageConversionJobData has to be set prior to this job execution");
    ImageConversionJobData* jobData = reinterpret_cast<ImageConversionJobData*>(GetJobData());

    assertm(jobData->m_outputData, "Output data should be created prior to this job execution");
    jobData->m_outputData->ConvertDataCPU(jobData->m_texels);
}
