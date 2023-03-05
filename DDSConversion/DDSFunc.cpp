#include "DDSFunc.h"

#include <JobManager/JobManager.h>

#include <DDSConversion/Jobs/ConvertImageJob.h>
#include <DDSConversion/Jobs/SplitImageJob.h>
#include <DDSConversion/Jobs/ImageConversionJobData.h>

#include <iostream>

void ConvertImagesIntoDDS(std::vector<std::pair<std::string, std::string>> imageFiles, CompressionType type)
{
    auto start = std::chrono::steady_clock::now();

    JobManager jobManager;
    std::vector<std::shared_ptr<JobData>> jobsData;
    for (const auto& pair : imageFiles)
    {
        std::shared_ptr<ImageConversionJobData> jobData = std::make_shared<ImageConversionJobData>(pair.first, pair.second, type);
        jobsData.push_back(jobData);

        std::shared_ptr<Job> splitJob = std::make_shared<SplitImageJob>(pair.first, jobData);

        std::shared_ptr<Job> convertJob = std::make_shared<ConvertImageJob>(jobData);
        convertJob->AddDependency(splitJob.get());

        jobManager.QueueJob(splitJob);
        jobManager.QueueJob(convertJob);
    }

    jobManager.Start();
    jobManager.Stop();

    for (const auto& data : jobsData)
    {
        ImageConversionJobData* jobData = reinterpret_cast<ImageConversionJobData*>(data.get());
        std::ofstream outputImage;
        outputImage.open(jobData->m_outputFile, std::ios::out | std::ios::binary);
        jobData->m_outputData->Serialize(outputImage);
    }

    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    std::cout << "Job manager: " << std::chrono::duration<double, std::milli>(diff).count() << " ms" << std::endl;
}
