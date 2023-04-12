#include "ImageConversionJobData.h"

ImageConversionJobData::~ImageConversionJobData()
{
    if (m_outputData)
    {
        delete m_outputData;
    }
}

ImageConversionJobData::ImageConversionJobData(std::string sourceFile, std::string outputFile, CompressionType compressionType, TextureOrientation textureOrientation)
    : m_sourceFile(sourceFile)
    , m_outputFile(outputFile)
    , m_compressionType(compressionType)
    , m_textureOrientation(textureOrientation)
{
}
