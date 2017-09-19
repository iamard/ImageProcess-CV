#ifndef __PYRAMID_H__
#define __PYRAMID_H__

#include "Gaussian.h"

class Pyramid {
public:
    virtual ~Pyramid() {
    }

    virtual size_t levels() const = 0;

protected:
    Pyramid() {
    }
};

/*
 * Gaussian pyramid class
 */
template <typename T>
class GPyramid: public Pyramid {
public:
    static GPyramid<T>* create(const Image<T> &image,
                               double         sigma,
                               double         factor,
                               int32_t        nScale) {
        GPyramid<T> *pyramid = new GPyramid<T>();

        for (int32_t index = 0; index < nScale; index++) {
            pyramid->add_image(new Image<T>(gaussian<T>(image, sigma, 5)));
            sigma *= factor;
        }

        return pyramid;
    }
    
    virtual ~GPyramid() {
        for (int32_t index = 0; index < array.size(); index++) {
            delete array[index];
        }
    }
    
    virtual size_t levels() const {
        return array.size();
    }

    Image<T>& getLevel(int32_t level) {
        return array[level];
    }

private:
    GPyramid() {
    }

    void add_image(Image<T>* image) {
        array.push_back(image);
    }

    vector<Image<T>*> array;
};

template <typename T>
class ScaleSpace: public Pyramid {
public:
    static ScaleSpace<T>* create(const Image<T> &image,
                                 int32_t        nOctave,
                                 double         sigma,
                                 double         factor,
                                 int32_t        nScale) {
        ScaleSpace<T>* pyramid = new ScaleSpace<T>();
        if (pyramid == nullptr) {
            return pyramid;
        }
        printf("copy before\n");
        Image<T> input  = image;
        printf("copy end\n");
        size_t   width  = input.width();
        size_t   height = input.height();
        for (int32_t index = 0; index < nOctave; index++) {
            GPyramid<T>* gaussian = GPyramid<T>::create(input,
                                                        sigma,
                                                        factor,
                                                        nScale);
            pyramid->add_gpyramid(gaussian);

            width  /= 2;
            height /= 2;
            if (width <= 0 || height <= 0)
                break;
            printf("test %d %d\n", input.width(), input.height());
            Image<T> input = resizeImiage<T>(image, width, height);
        }

        return pyramid;
    }

    virtual ~ScaleSpace() {
        for (int32_t index = 0; index < array.size(); index++) {
            delete array[index];
        }
    }

    virtual size_t levels() const {
        return array.size();
    }

private:
    ScaleSpace() {
    }

    void add_gpyramid(GPyramid<T> *pyramid) {
        array.push_back(pyramid);
    }

    vector<GPyramid<T>*> array;
};

template <typename T>
class DoGPyramid: public Pyramid {
public:
    static DoGPyramid<T>* create(const GPyramid<T>& gaussian) {
        DoGPyramid<T>* pyramid = new DoGPyramid<T>();
        if (pyramid == nullptr) {
            return nullptr;
        }

        size_t levels = gaussian.levels();
        for (int32_t index = 0; index < levels; index++) {
            //Image<T> image = gaussian.getLevel(index) - gaussian.getLevel(index + 1);
            //pyramid->add_diff(new Image<T>(image));
        }
        
        return pyramid;
    }

    ~DoGPyramid() {
        for (int32_t index = 0; index < array.size(); index++) {
            delete array[index];
        }       
    }

    virtual size_t levels() const {
        return array.size();
    }

private:
    void add_diff(Image<T>* image) {
        array.push_back(image);
    }

    vector<Image<T>*> array;    
};

#endif  // __PYRAMID_H__
