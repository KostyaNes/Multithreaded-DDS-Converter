#include "SplitImageJob.h"

#include <ProjectDefines.h>

#include <DDSConversion/Textures/ImageUtils.h>
#include <DDSConversion/Textures/TextureBlock.h>

#include <QImage>

#include "ImageConversionJobData.h"

SplitImageJob::SplitImageJob(std::string sourceImage, std::shared_ptr<ImageConversionJobData>& jobData)
{
    m_sourceImage = new QImage(sourceImage.c_str());
    AddJobData(jobData);
}

SplitImageJob::~SplitImageJob()
{
    if (m_sourceImage)
    {
        delete m_sourceImage;
    }
}

void SplitImageJob::Execute()
{
    assertm(GetJobData(), "ImageConversionJobData has to be set prior to job execution");
    ImageConversionJobData* jobData = reinterpret_cast<ImageConversionJobData*>(GetJobData());

    ConvertQImageToTexelArray(*m_sourceImage, jobData->m_texels);
    jobData->m_outputData = new TextureData(jobData->m_compressionType, m_sourceImage->height(), m_sourceImage->width());

    delete m_sourceImage;
    m_sourceImage = nullptr;
}
