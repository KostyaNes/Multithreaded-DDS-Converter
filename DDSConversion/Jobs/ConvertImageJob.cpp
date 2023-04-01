#include "ConvertImageJob.h"

#include <ProjectDefines.h>

#include <DDSConversion/Jobs/ImageConversionJobData.h>

ConvertImageJob::ConvertImageJob(std::shared_ptr<ImageConversionJobData> &jobData)
{
    AddJobData(jobData);
}

ConvertImageJob::ConvertImageJob(std::shared_ptr<ImageConversionJobData> &jobData, int texelBegin, int texelEnd)
{
    AddJobData(jobData);
    m_texelBegin = texelBegin;
    m_texelEnd = texelEnd;
}

void ConvertImageJob::Execute()
{
    assertm(GetJobData(), "ImageConversionJobData has to be set prior to this job execution");
    ImageConversionJobData* jobData = reinterpret_cast<ImageConversionJobData*>(GetJobData());

    assertm(jobData->m_outputData, "Output data should be created prior to this job execution");
    jobData->m_outputData->ConvertDataCPU(jobData->m_texels);
}
