#pragma once
#include <string>
#include "image.h"

// Function to load a PGM image
Image<int> load_pgm (const std::string& filename1);

// Struct to hold Contrast data ( contrast agent and dose)
struct ContrastData{
    std::string contrast_agent;
    double dose;
};

// Struct to hold Contrast data and argument if data was loaded
struct ContrastDataLoad {
    ContrastData data;
    bool loaded;
};

// Function to load contrast data
ContrastData load_contrast (const std::string& filename2);

// Function to check if contrast data file is provided
ContrastDataLoad load_contrast_data_from_args(std::vector<std::string>& args);
