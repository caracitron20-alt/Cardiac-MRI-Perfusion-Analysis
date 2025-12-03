// Derived Class

#pragma once

#include "TemporalDataset.h"
#include "load_parameters.h" 
#include "debug.h"

// Structure to store the frame and signal values for find_peak_contrast 
// and find_arrival_contrast methods
struct Contrast {
    int frame;
    double signal;
};

// Class for perfusion image dataset derived from temporal dataset
template <typename ValueType>
class PerfusionImageDataset : public TemporalDataset<ValueType> {
    public:
    // Constructors
    PerfusionImageDataset() = default;
    PerfusionImageDataset(const std::vector<std::string>& filenames) : 
    TemporalDataset<ValueType>(filenames) {}

    // Methods for:
    // Calculating temporal gradient (G)
    std::vector<double> calculate_temporal_gradient(const std::vector<double>& signal) const;

    // Finding peak contrast (frame and signal)
    Contrast find_peak_contrast(const std::vector<double>& signal);
    
    // Finding the arrival contrast (frame and signal) for a given treshold value
    Contrast find_arrival_contrast(const std::vector<double>& signal, 
        const std::vector<double>& gradient, int peak_frame, double threshold);

    // Find the temporal gradient (G) of the signal during contrast uptake
    double calculate_temporal_uptake_gradient(double peak_signal, double arrival_signal, 
        int peak_frame, int arrival_frame);
};


// Implementation of method to computes temporal gradient (G)
template <typename ValueType>
std::vector<double> PerfusionImageDataset<ValueType>::calculate_temporal_gradient(
    const std::vector<double>& signal) const {
    
    // Initialise gradient vector
    std::vector<double> gradient(signal.size());
    
    for (unsigned int i = 0; i < gradient.size(); ++i) {
        gradient[i] = signal[i + 1] - signal[i];
    }
    
    // Pad with 0.0 at then end 
    gradient[signal.size() - 1] = 0.0;
    debug::log(std::format("Computed {} gradients succesfully",gradient.size()));
    return gradient;
    
        
}

// Implementation of method to identify the time frame of peak contrast 
// concentration in the LVBP region and the corresponding mean LVBP signal 
// in the image at that time frame
template <typename ValueType>
Contrast PerfusionImageDataset<ValueType>::find_peak_contrast(
    const std::vector<double>& signal) {
    if (signal.empty()) {
        throw std::runtime_error("Empty signal vector");
    }
    int peak_frame = 0;
    double peak_value = signal[0];
    
    for (int d = 1; d < signal.size(); ++d) {
        if (signal[d] > peak_value) {
            peak_value = signal[d];
            peak_frame = d;
        }
    }
    
    if (peak_frame!=0){
        debug::log(std::format("Found peak frame succesfully"));
    }
    // Return frame and signal
    return {peak_frame, peak_value};
    
}

// Implementation of method to identify the time frame of contrast arrival 
// in LVBP and the corresponding LVBP signal in the image at that time frame
template <typename ValueType>
Contrast PerfusionImageDataset<ValueType>::find_arrival_contrast(
    const std::vector<double>& signal, const std::vector<double>& gradient, 
    int peak_frame, double threshold) {
    
    // Initialise frame
    int arrival_frame = 0;
    
    // Look for the earliest time frame before peak where gradient exceeds threshold
    for (int d = 0; d < peak_frame && d < gradient.size(); ++d) {
        if (gradient[d] > threshold) {
            arrival_frame = d;
            break;
        }
    }
    if (arrival_frame!=0){
        debug::log(std::format("Found arrival frame succesfully"));
    }
    // Return frame and signal
    return {arrival_frame, signal[arrival_frame]};
}

// Implementation of method to compute the temporal gradient (G) of the signal during contrast uptake
template <typename ValueType>
double PerfusionImageDataset<ValueType>::calculate_temporal_uptake_gradient(double peak_signal, 
    double arrival_signal, int peak_frame, int arrival_frame) {
    // Avoid division by zero
    if (peak_frame == arrival_frame) {
        return 0.0; 
    }
    double uptake_gradient = (peak_signal - arrival_signal) / (peak_frame - arrival_frame);
   
    // Return uptake gradient
    return uptake_gradient;
}