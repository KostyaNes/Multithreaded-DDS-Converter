#include "ImageConversionJobData.h"

ImageConversionJobData::~ImageConversionJobData()
{
    if (m_outputData)
    {
        delete m_outputData;
    }
}

ImageConversionJobData::ImageConversionJobData(std::string sourceFile, std::string outputFile, CompressionType compressionType)
    : m_sourceFile(sourceFile)
    , m_outputFile(outputFile)
    , m_compressionType(compressionType)
{
}
