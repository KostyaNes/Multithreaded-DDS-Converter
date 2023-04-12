#pragma once

#include <string>
#include <vector>

#include <DDSConversion/DDSDefines.h>

class JobManager;

void ConvertImagesIntoDDS(std::vector<std::pair<std::string, std::string>> imageFiles, CompressionType type, TextureOrientation textureOrientation);
void CreateJobForImageConversion(JobManager& jobManager, std::string& inputFile, std::string& outputFile, CompressionType type, TextureOrientation textureOrientation);

void ReadDDSTexture(std::string filePath, Texture& outTexture);
