#pragma once

#include <JobManager/JobData.h>

#include <DDSConversion/DDSDefines.h>
#include <DDSConversion/Textures/ImageTexel.h>
#include <DDSConversion/Textures/TextureData.h>

#include <vector>

class ImageConversionJobData
        : public JobData
{
public:
    ~ImageConversionJobData();

    ImageConversionJobData(std::string sourceFile, std::string outputFile, CompressionType compressionType, TextureOrientation textureOrientation);

public:
    std::string m_sourceFile;
    std::string m_outputFile;

    CompressionType m_compressionType { CompressionType::DXT1 };
    TextureOrientation m_textureOrientation { TextureOrientation::DirectX };

    std::vector<ImageTexel> m_texels;
    TextureData* m_outputData { nullptr };
};

