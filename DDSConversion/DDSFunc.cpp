#include "DDSFunc.h"

#include <JobManager/JobManager.h>

#include <DDSConversion/Jobs/ConvertImageJob.h>
#include <DDSConversion/Jobs/SplitImageJob.h>
#include <DDSConversion/Jobs/SaveTextureJob.h>
#include <DDSConversion/Jobs/ImageConversionJobData.h>

#include <iostream>

void ConvertImagesIntoDDS(std::vector<std::pair<std::string, std::string>> imageFiles, CompressionType type)
{
    auto start = std::chrono::steady_clock::now();

    JobManager jobManager;
    JobManager::SetupBasicConfigurationWithIOThread(jobManager);

    for (auto& pair : imageFiles)
    {
        CreateJobForImageConversion(jobManager, pair.first, pair.second, type);
    }

    jobManager.Start();
    jobManager.Stop();

    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    std::cout << "Job manager: " << std::chrono::duration<double, std::milli>(diff).count() << " ms" << std::endl;
}

void CreateJobForImageConversion(JobManager &jobManager, std::string &inputFile, std::string &outputFile, CompressionType type)
{
    std::shared_ptr<ImageConversionJobData> jobData = std::make_shared<ImageConversionJobData>(inputFile, outputFile, type);

    std::shared_ptr<Job> splitJob = std::make_shared<SplitImageJob>(inputFile, jobData);

    std::shared_ptr<Job> convertJob = std::make_shared<ConvertImageJob>(jobData);
    convertJob->AddDependency(splitJob.get());

    std::shared_ptr<Job> saveTextureJob = std::make_shared<SaveTextureJob>(jobData);
    saveTextureJob->AddDependency(convertJob.get());

    jobManager.QueueJob(splitJob);
    jobManager.QueueJob(convertJob);
    jobManager.QueueJob(saveTextureJob);
}
