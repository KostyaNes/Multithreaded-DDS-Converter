#pragma once

#include <string>
#include <vector>

#include <DDSConversion/DDSDefines.h>

void ConvertImagesIntoDDS(std::vector<std::pair<std::string, std::string>> imageFiles, CompressionType type);
