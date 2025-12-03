#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <filesystem>

#include "load_parameters.h"

#include "debug.h"
#include "image.h"
#include "terminal_graphics.h"
#include "mask.h"
#include "PerfusionImageDataset.h"
// This function contains our program's core functionality which is to analyse 
// cardiac MRI perfusion data:

void run (std::vector<std::string>& args)
{
  debug::verbose = std::erase (args, "-v");
    
  // Check for minimum required arguments
  if (args.size() < 2)
      throw std::runtime_error ("missing arguments - expected at least 1 argument");

  // Load contrast data if available file otherwise continue without, using 
  // load_contrast_data_from_args function
  auto [contrast_data, contrast_data_loaded] = load_contrast_data_from_args(args);
  
  // Load image data using data function
  PerfusionImageDataset<int> data ({args.begin()+1, args.end()});
  
  // Create 2D binary image using create_mask function
  // Initialise variables with given values:
  SquareMask square_mask(data[0].width(), data[0].height(), 74, 90, 5);
  Image<float> lvbp_mask(data[0].width(), data[0].height());
  // Apply function:
  square_mask.create_mask(lvbp_mask);
  
  // Get average signal S(d), signal timecourse
  std::vector<double> average_signals;
  for (unsigned int i = 0; i < data.size(); ++i) {
      average_signals.push_back(data[i].calculate_average_signal(lvbp_mask));
  }

  // Calculate temporal gradient using class method calculate_temporal_gradient
  std::vector<double> gradient = data.calculate_temporal_gradient(average_signals);
  
  // Find peak contrast (frame and signal) using find_peak_contrast method
  auto [peak_frame, peak_signal] = data.find_peak_contrast(average_signals);
  
  // Find the arrival contrast (frame and signal) for a given treshold value using 
  // find_arrival_contrast method
  double threshold = 10.0;
  auto [arrival_frame, arrival_signal] = data.find_arrival_contrast(average_signals, 
    gradient, peak_frame, threshold);
  
  // Find the temporal gradient (G) of the signal during contrast uptake using 
  // calculate_temporal_uptake_gradient method
  double uptake_gradient = data.calculate_temporal_uptake_gradient(peak_signal, 
    arrival_signal, peak_frame, arrival_frame);
 
  // Print everything to terminal
  // Plots & Images
  // Display the image frame at oeak contrast concentration 
  std::cout<<"Image at peak contrast concentration: \n";
  TG::imshow (TG::magnify (data[peak_frame], 3), 0, 200);

  // Plot average signal S(d) in LVBP region for each time frame d
  std::cout<<"Signal timecourse within ROI: \n";
  TG::plot()
    .add_line(average_signals)
    .set_grid(2,20);

  // Plot the gradient timecourse 
  std::cout<<"Gradient of signal timecourse within ROI: \n";
  TG::plot()
    .add_line(gradient,3)
    .set_grid(2,20);
  
  // Display data
  // Display contrast agent info
  if (!contrast_data.contrast_agent.empty() && contrast_data.dose > 0){
    std::cout << std::format ( "Contrast agent: {} , dose = {} mmol/kg\n ",
      contrast_data.contrast_agent, contrast_data.dose);
    } else {
    std::cout << "No contrast agent data available\n";
  }

  // Display contrast arrival (frame & signal)
  std::cout << std::format ( "Contrast arrival occurs at time frame {}, with signal intensity: {}\n ",
    arrival_frame,arrival_signal);

  // Display peak contrast concentration (frame & signal)
  std::cout << std::format ( "Peak contrast concentration occurs at time frame {}, with signal intensity: {}\n ",
    peak_frame,peak_signal);

  // Display Temporal gradient of signal during contrast update
  std::cout << std::format ( "Temporal gradient of signal during contrast update: {}\n ",
    uptake_gradient);

}

// skeleton main() function, whose purpose is now to pass the arguments to
// run() in the expected format, and catch and handle any exceptions that may
// be thrown.

int main (int argc, char* argv[])
{
  try {
    std::vector<std::string> args (argv, argv+argc);
    run (args);
  }
  catch (std::exception& excp) {
    std::cerr << "ERROR: " << excp.what() << " - aborting\n";
    return 1;
  }
  catch (...) {
    std::cerr << "ERROR: unknown exception thrown - aborting\n";
    return 1;
  }
  return 0;
}
