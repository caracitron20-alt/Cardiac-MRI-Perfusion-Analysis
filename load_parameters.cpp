#include <iostream>
#include <fstream>
#include <stdexcept>
#include <format>

#include "debug.h"
#include "load_parameters.h"
#include "image.h"

// Implementation of function to load a PGM image
Image<int> load_pgm (const std::string& filename1)
{   
    debug::log ("loading PGM file \"" + filename1 + "\"...");
    
    std::ifstream infile1 (filename1);
    if (!infile1)
        throw std::runtime_error ("unable to open file \"" + filename1 + "\"");
    
    // Check the file trying to be loaded is in pgm format, otherwise ERROR 
    std::string magic_number;
    infile1 >> magic_number;
    if (magic_number != "P2")
        throw std::runtime_error ("file\"" + filename1 + "\"is not in plain PGM format");

    // Read in file data ( dimensions and maximum value)
    int xdim, ydim, maxval;
    infile1 >> xdim >> ydim >> maxval;

    debug::log(std::format ( "PGM file \"{}\" contains {}x{} image with maximum value {}",filename1,xdim,ydim,maxval));

    // Read in pixel data
    int val; 
    std::vector<int> data;
    while(infile1 >> val)
        data.push_back(val);

    if (static_cast<int>(data.size()) != xdim * ydim)
        throw std::runtime_error (std::format("amount of data in PGM file \"{}\" ({}) does not match dimensions ({}x{})", filename1, data.size(), xdim, ydim));
    
    // Create and return Image object
    Image image (xdim,ydim,data);
    return image;
}


// Implementation of function to load contrast data
ContrastData load_contrast (const std::string& filename2){
    debug::log ("loading contrast file \"" + filename2 + "\"...");
    
    std::ifstream infile2 (filename2);
    if (!infile2)
        throw std::runtime_error ("unable to open file \"" + filename2 + "\"");
    
    // Initialise contrast data struct
    ContrastData m_data;
    
    // Read first line of file and load as contrast name
    std::getline(infile2,m_data.contrast_agent);
    if (m_data.contrast_agent.empty())
        throw std::runtime_error("file \"" + filename2 + "\" does not have dose value");
    size_t pos = m_data.contrast_agent.find_last_not_of(" \t\n\r");
    if (pos != std::string::npos) {
        m_data.contrast_agent.erase(pos + 1);
    } else {
        m_data.contrast_agent.clear(); // String contains only whitespace
    }
    
    // Read in second data in file as dose
    infile2 >> m_data.dose;
    
    debug::log(std::format ( "Contrast file contains {} contrast agent with dose of {} mmol/kg ",filename2,m_data.contrast_agent,m_data.dose));

    return m_data;
}

// Implementation of function to check if contrast data file is provided,
// if so load its data, otherwise continue.
ContrastDataLoad load_contrast_data_from_args(std::vector<std::string>& args) {
    // Check if there is a contrast text file argument 
    std::string contrast_file = "";
    auto it = std::ranges::find_if(args, [](const std::string& arg) {
        return arg.find(".txt") != std::string::npos;
    });
      
    if (it != args.end()) {
        contrast_file = *it;
        args.erase(it); // Remove the contrast file from the image list
    }
      
    // If there is a Contrast Text File argument then load Contrast Data file into variable
    ContrastData contrast_data;
    bool contrast_data_loaded = false;
    if (!contrast_file.empty()) {
        try {
        // Check if the file exists, if not continue without it
        if (std::filesystem::exists(contrast_file)) {
            // Loading contrast agent into struct using load_contrast function
            contrast_data = load_contrast(contrast_file);
            debug::log(std::format("Loaded contrast agent: {} (dose = {:.3f} mmol/kg)", 
            contrast_data.contrast_agent, contrast_data.dose));
            contrast_data_loaded = true;
        } else {
            std::cout << "Warning: no contrast data file " << contrast_file << 
            ". Continuing without it.\n";
        }
        } catch (const std::exception& e) {
        debug::log(std::format("Failed to load contrast data: {}", e.what()));
        std::cout << "Warning: Could not load contrast data file. Continuing without it.\n";
        }
    } else {
        std::cout << "Warning: no contrast data file. Continuing without it.\n";
    }
    return {contrast_data,contrast_data_loaded};
}
  