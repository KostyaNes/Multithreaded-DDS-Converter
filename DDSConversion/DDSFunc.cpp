#include "DDSFunc.h"

#include <JobManager/JobManager.h>

#include <DDSConversion/Jobs/ConvertImageJob.h>
#include <DDSConversion/Jobs/SplitImageJob.h>
#include <DDSConversion/Jobs/SaveTextureJob.h>
#include <DDSConversion/Jobs/ImageConversionJobData.h>

#include <iostream>

void ConvertImagesIntoDDS(std::vector<std::pair<std::string, std::string>> imageFiles, CompressionType type, TextureOrientation textureOrientation)
{
    auto start = std::chrono::steady_clock::now();

    JobManager jobManager;
    JobManager::SetupBasicConfigurationWithIOThread(jobManager);

    for (auto& pair : imageFiles)
    {
        CreateJobForImageConversion(jobManager, pair.first, pair.second, type, textureOrientation);
    }

    jobManager.Start();
    jobManager.Stop();

    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    std::cout << "Job manager: " << std::chrono::duration<double, std::milli>(diff).count() << " ms" << std::endl;
}

void CreateJobForImageConversion(JobManager &jobManager, std::string &inputFile, std::string &outputFile, CompressionType type, TextureOrientation textureOrientation)
{
    std::shared_ptr<ImageConversionJobData> jobData = std::make_shared<ImageConversionJobData>(inputFile, outputFile, type, textureOrientation);

    std::shared_ptr<Job> splitJob = std::make_shared<SplitImageJob>(inputFile, jobData);

    /*
    std::shared_ptr<Job> convertJob = std::make_shared<ConvertImageJob>(jobData);
    convertJob->AddDependency(splitJob.get());
    */

    /*
    std::shared_ptr<Job> convertJobFirst = std::make_shared<ConvertImageJob>(jobData, 0, 524288);
    convertJobFirst->AddDependency(splitJob.get());
    std::shared_ptr<Job> convertJobSecond = std::make_shared<ConvertImageJob>(jobData, 524288, 1048576);
    convertJobSecond->AddDependency(splitJob.get());

    std::shared_ptr<Job> saveTextureJob = std::make_shared<SaveTextureJob>(jobData);
    saveTextureJob->AddDependency(convertJobFirst.get());
    saveTextureJob->AddDependency(convertJobSecond.get());
    */

    std::shared_ptr<Job> convertJob1 = std::make_shared<ConvertImageJob>(jobData, 0, 262144);
    convertJob1->AddDependency(splitJob.get());
    std::shared_ptr<Job> convertJob2 = std::make_shared<ConvertImageJob>(jobData, 262144, 524288);
    convertJob2->AddDependency(splitJob.get());
    std::shared_ptr<Job> convertJob3 = std::make_shared<ConvertImageJob>(jobData, 524288, 786432);
    convertJob3->AddDependency(splitJob.get());
    std::shared_ptr<Job> convertJob4 = std::make_shared<ConvertImageJob>(jobData, 786432, 1048576);
    convertJob4->AddDependency(splitJob.get());

    std::shared_ptr<Job> saveTextureJob = std::make_shared<SaveTextureJob>(jobData);
    saveTextureJob->AddDependency(convertJob1.get());
    saveTextureJob->AddDependency(convertJob2.get());
    saveTextureJob->AddDependency(convertJob3.get());
    saveTextureJob->AddDependency(convertJob4.get());

    jobManager.QueueJob(splitJob);
    jobManager.QueueJob(convertJob1);
    jobManager.QueueJob(convertJob2);
    jobManager.QueueJob(convertJob3);
    jobManager.QueueJob(convertJob4);
    jobManager.QueueJob(saveTextureJob);
}

void ReadDDSTexture(std::string filePath, Texture& outTexture)
{
    std::ifstream file(filePath, std::ios::in | std::ios::binary);

    char* buffer = new char[sizeof(DDS_HEADER)];

    file.read(buffer, sizeof(DDS_HEADER));

    DDS_HEADER* header = reinterpret_cast<DDS_HEADER*>(buffer);
    outTexture.m_header = *header;

    int dataSizeMultiplier = 0;

    const DWORD dxt1 = ('D' << 0) | ('X' << 8) | ('T' << 16) | ('1' << 24);
    const DWORD dxt3 = ('D' << 0) | ('X' << 8) | ('T' << 16) | ('3' << 24);
    const DWORD dxt5 = ('D' << 0) | ('X' << 8) | ('T' << 16) | ('5' << 24);

    switch (header->ddspf.dwFourCC)
    {
        case (dxt1):
        {
            dataSizeMultiplier = 8;
            break;
        }
        case(dxt3):
        {
            dataSizeMultiplier = 16;
            break;
        }
        case(dxt5):
        {
            dataSizeMultiplier = 16;
            break;
        }
    }

    unsigned long long dataSize = (header->dwWidth + (4 - header->dwWidth % 4) % 4) * (header->dwHeight + (4 - header->dwHeight % 4) % 4) / 16 * dataSizeMultiplier;

    if (dataSize > 0)
    {
        if (outTexture.m_data)
        {
            delete[] outTexture.m_data;
        }

        outTexture.m_data = new char[dataSize];
        file.read(outTexture.m_data, dataSize);
    }

    delete[] buffer;
    file.close();
}
