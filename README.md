# Cardiac-MRI-Perfusion-Analysis
C++ program to load, analyse, and visualise cardiac MRI perfusion data 

This code implements the folllowing functionalities: 
1. Load one time series of cardiac MRI perfusion images from a series of PGM image files
(saved as “mri-01.pgm”, “mri-02.pgm”, etc).
2. Load the contrast agent information from a separate file called “contrast_info.txt”.
3. Create a 2D binary image (i.e. a mask, where pixels can only take values 0 or 1) of the same
size as the original image, where all pixels are set to 1 if inside an LVBP, or 0 otherwise.
The LVBP region should be defined as a 5x5 pixel square centred on the pixel at position
(x, y) = (74, 90) in the given example.
4. Calculate the average signal S(d) in the LVBP region for each time frame d, and plot this
timecourse on the terminal.
5. Identify the time frame of peak contrast concentration (d_peakBlood) in the LVBP region and
the corresponding mean LVBP signal (S_peakBlood) in the image at that time frame. Display
the image frame at peak contrast concentration in the terminal.
6. Identify the time frame of contrast arrival d_arrival in LVBP and the corresponding LVBP
signal (S_arrival) in the image at that time frame. This should be computed as the earliest
time frame (prior to d_peakBlood) at which the temporal gradient of S(d) (the signal within the
LVBP region) exceeds the threshold value of 10. The temporal gradient should be
computed as S(d+1)-S(d). Plot the gradient timecourse on the terminal.
7. Compute the temporal gradient (G) of the signal during contrast uptake as:
G = (SpeakBlood – Sarrival)/(dpeakBlood – darrival).
8. Display the contrast agent info associated with this scan as well as d_arrival, S_arrival, d_peakBlood,S_peakBlood, and G.
