#pragma once

#include "image.h"

// Base Class
class Mask{
    public:
        // Constructors
        Mask(int width, int height);
        virtual ~Mask() = default;

        // Create pure virtual methods for creating specific mask
        virtual void create_mask(Image<float>& mask)=0;

        // Getters
        int find_width() const ;
        int find_height() const;
    protected:
        int width;
        int height;
};

// Derived class
class SquareMask : public Mask {
    public: 
        SquareMask(int width, int height, int x_center_pixel,int y_center_pixel, int mask_size);

        // Create a 2D mask
        void create_mask(Image<float>& mask) override;

    private:
        int x_center_pixel;
        int y_center_pixel;
        int mask_size;
};