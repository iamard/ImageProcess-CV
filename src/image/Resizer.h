#ifndef __RESIZER_H__
#define __RESIZER_H__

template<typename T>
inline double bilinear(const Image<T> &image, 
                       double         inputX,
                       double         inputY) {
    int32_t intX  = (int32_t)inputX;
    int32_t intY  = (int32_t)inputY;
    double  fracX = inputX - intX;
    double  fracY = inputY - intY;

    if (T::channels == 1) {
        int32_t pixel1 = image[intX + 0][intY + 0].data[0];
        int32_t pixel2 = image[intX + 0][intY + 1].data[0];
        int32_t pixel3 = image[intX + 1][intY + 0].data[0];
        int32_t pixel4 = image[intX + 1][intY + 0].data[0];
        
        double upper = pixel1 * (1.0 - fracX) + pixel3 * fracX; 
        double lower = pixel2 * (1.0 - fracX) + pixel4 * fracX;
        double value = upper * (1.0 - fracY) + lower * fracY;

        // Clamp the output value
        value += 0.5;
        if (value > 255) {
            value = 255;
        } else if (value < 0) {
            value = 0;
        }

        return value;
    }
    
    if (T::channels == 3) {
        // TBD
    }
}

template<typename T>
Image<T> resizeImiage(const Image<T> &image, 
                     size_t         outputW,
                     size_t         outputH) {
    double inputW = image.width();
    double inputH = image.height();
    double ratioW = inputW / (double)outputW;
    double ratioH = inputH / (double)outputH;

    Image<T> output(outputW, outputH);
    for (int32_t outY = 0; outY < outputH; outY++) {
        for (int32_t outX = 0; outX < outputW; outX++) {
            double inX = outX * ratioW;
            double inY = outY * ratioH;
            double val = bilinear(image, inX, inY);
            output[outX][outY].data[0] = (int32_t)val;
        }
    }

    return output;
}

#endif  // __RESIZER_H__
