#ifndef __GPU_DEVICE_H__
#define __GPU_DEVICE_H__

#include <CL/cl.h>
#include "Common.h"

class GPUDevice {
public:
    static GPUDevice* create() {
        return new GPUDevice();
    }

    virtual ~GPUDevice() {
        if (command_ != 0)
            clReleaseCommandQueue(command_);

        if (context_ != 0)
            clReleaseContext(context_);
    }

private:
    void createContext() {
        cl_int         errorCode;
        cl_uint        numPlatforms;
        cl_platform_id platformId;

        errorCode = clGetPlatformIDs(1, &platformId, &numPlatforms);
        if (errorCode != CL_SUCCESS || numPlatforms <= 0) {
            return;
        }
        
        cl_context_properties contextProperties[] = {
            CL_CONTEXT_PLATFORM,
            (cl_context_properties)platformId,
            0
        };

        context_ = clCreateContextFromType(contextProperties,
                                           CL_DEVICE_TYPE_GPU,
                                           NULL,
                                           NULL,
                                           &errorCode);
        if (errorCode != CL_SUCCESS) {
            context_ = clCreateContextFromType(contextProperties,
                                               CL_DEVICE_TYPE_CPU,
                                               NULL,
                                               NULL,
                                               &errorCode);
            if (errorCode != CL_SUCCESS) {
                return;
            }
        }
    }

    void queryDevices() {
        cl_int errorCode;
        size_t bufferSize = -1;

        // First get the size of the devices buffer
        errorCode = clGetContextInfo(context_, CL_CONTEXT_DEVICES, 0, NULL, &bufferSize);
        if (errorCode != CL_SUCCESS) {
            return;
        }

        if (bufferSize <= 0) {
            return;
        }

        // Allocate memory for the devices buffer
        devCount_ = bufferSize / sizeof(cl_device_id);
        devices_  = new cl_device_id[devCount_];
        errorCode = clGetContextInfo(context_, CL_CONTEXT_DEVICES, bufferSize, devices_, NULL);
        if (errorCode != CL_SUCCESS) {
            delete[] devices_;
            return;
        }
    }

    void createCommand() {
        // In this example, we just choose the first available device.  In a
        // real program, you would likely use all available devices or choose
        // the highest performance device based on OpenCL device queries
        command_ = clCreateCommandQueue(context_, devices_[0], 0, NULL);
        if (command_ == NULL) {
            delete[] devices_;
            return;
        }
    }

    GPUDevice()
        : context_(0),
          devices_(0),
          command_(0) {
        createContext();
        queryDevices();
        createCommand();
    }

    cl_context       context_;
    size_t           devCount_;
    cl_device_id     *devices_;
    cl_command_queue command_;
};

#endif  // __GPU_DEVICE_H__
