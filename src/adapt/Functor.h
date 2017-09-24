#ifndef __FUNCTOR_H__
#define __FUNCTOR_H__

#include "Precision.h"
#include "GPUDevice.h"

template<typename T, typename D = GPUDevice>
class Convolver {
    using Function = Image<T> (*)(const Image<T> &image, const FracType<T> *kernel, size_t n);

public:
    Convolver(Function function)
        : function_(function) {
    }

    virtual ~Convolver() {
    }

    Image<T> dispatch(const Image<T> &image, const FracType<T> *kernel, size_t n) {
        //device_.createProgram();
        
        //device_.createKernel();

        //device_.setParameter();
        
        return function_(image, kernel, n);
    }

private:
    Function function_;
    //Device   device_;
};

#endif  // __FUNCTOR_H__
