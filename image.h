#pragma once
#include <array>
#include <vector>
#include <stdexcept>

// Template image class
template <typename T>
class Image {
    public:
        // Constructors:
        Image (int xdim, int ydim):
            m_dim {xdim,ydim},m_data(xdim*ydim,0){}
        
        Image (int xdim,int ydim, const std::vector<T>& data):
            m_dim{xdim,ydim},m_data(data){
                if (static_cast<int>(m_data.size())!=m_dim[0]*m_dim[1])
                    throw std::runtime_error ("dimensions do not match data vector");

            }
        
        // Getters
        int width() const{ return m_dim[0]; }
        int height() const { return m_dim[1]; }

        // Operators
        T&       operator() (int x, int y)       { return m_data[x + m_dim[0]*y]; }
        const T& operator() (int x, int y) const { return m_data[x + m_dim[0]*y]; }
        
        // Calculate average signal intensity within masked region
        double calculate_average_signal(const Image<float>& mask) const {
            // Verify dimensions
            if (width() != mask.width() || height() != mask.height()) {
                throw std::runtime_error("Image: Mask dimensions do not match image dimensions.");
            }
            //Initialise
            double sum = 0.0;
            int count = 0;
            // Iterate through all pixels in y and x directions
            for (int y = 0; y < height(); ++y) {
                for (int x = 0; x < width(); ++x) {
                    // Include pixel in average if in mask LVBP region
                    if (mask(x, y) == 1.0f) {
                        sum += static_cast<double>(operator()(x, y));
                        count++;
                    }
                }
            }
            // Return average
            return (count > 0) ? (sum / count) : 0.0;
        }
    private:
        std::array<int,2> m_dim; // Image dimensions
        std::vector<T> m_data; // Signal intensity of pixels (in row-major order)
};


