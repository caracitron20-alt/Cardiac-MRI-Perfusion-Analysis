#include "mask.h"
#include <iostream>
#include "image.h"
#include "debug.h"
// Base class implementation

// Constructor for the Mask class
Mask::Mask(int width, int height) : width(width), height(height) {}

// Getter method to retrieve the width of the mask
int Mask::find_width() const { 
    return width; 
}

// Getter method to retrieve the height of the mask
int Mask::find_height() const { 
    return height; 
}


// Derived Class implementation

// Constructor for the SquareMask class
SquareMask::SquareMask(int width, int height, int x_center_pixel, int y_center_pixel, int mask_size)
    : Mask(width, height), x_center_pixel(x_center_pixel), y_center_pixel(y_center_pixel), mask_size(mask_size) {}

    // Method to create a 2D square mask within a given Image
void SquareMask::create_mask(Image<float>& mask) {
    if (x_center_pixel < 0 || y_center_pixel < 0) {
        throw std::runtime_error ( "Error: Mask center coordinates must be >0 .\n");
        return;
    }
    // Calculate the step size
    int step_size=mask_size / 2;
    
    // Iterate through the rows and columns within the mask region
    for (int y=y_center_pixel-step_size; y<=y_center_pixel +step_size; y++){
        if (y < 0 || y >= height) continue;
        for (int x=x_center_pixel-step_size ; x<=x_center_pixel+step_size; x++){
            if (x < 0 || x >= width) continue;
            if (x >= 0 && x < width && y >= 0 && y < height) {
                mask(x,y)=1.0f;
            }
        }
    }
    debug::log (std::format ("Created a 2D mask of size {}x{}", 
        mask.width(), mask.height()));
}


