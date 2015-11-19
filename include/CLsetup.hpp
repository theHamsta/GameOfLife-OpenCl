#ifndef __CLSETUP_HPP
#define __CLSETUP_HPP



#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <stdexcept>
#include <utility>
#include "CLheader.hpp"


using std::string;
using std::vector;
using std::map;
using std::ostream;


/**
 *	\brief	OpenCL setup class.
 */
class SetupCL
{

private:

    // Avoid Copy
    SetupCL (const SetupCL &other);
    SetupCL& operator=(const SetupCL &other);

public:
    
    SetupCL ();
    
    ~SetupCL ();

    /**
     *	\brief	Returns a platform vector.
     */
    vector<cl::Platform>
    getPlatforms(void) const;

    /**
     *	\brief	Selects one platform.
     *	        This function provides user interaction and debugging output. 
     */
    cl::Platform
    selectPlatform(void) const;

    /**
     *	\brief 	Return a context from properties.
     */
    cl::Context
    createContext(cl_context_properties properties[],
                  const cl_device_type deviceType = CL_DEVICE_TYPE_ALL) const;

    /**
     *	\brief 	Return a context from platform.
     */
    cl::Context
    createContext(const cl::Platform &platform,
                  const cl_device_type deviceType = CL_DEVICE_TYPE_ALL) const;

    /**
     *	\brief	Returns devices vector from context.
     */
    vector<cl::Device>
    getDevices(const cl::Context &context) const;

    /**
     *	\brief	Returns devices vector from platform.
     */
    vector<cl::Device>
    getDevices(const cl::Platform &platform,
               const cl_device_type deviceType) const;

    /**
     *	\brief	Selects one device from context. 
     *	        This function provides user interaction and debugging output. 
     */
    cl::Device
    selectDevice(const cl::Context &context) const;

    /**
     *	\brief	Selects one device from platform.
     *	        This function provides user interaction and debugging output. 
     */
    cl::Device
    selectDevice(const cl::Platform &platform,
                 const cl_device_type deviceType = CL_DEVICE_TYPE_ALL) const;

    /**
     *	\brief	Creates a program from kernel filenames, context and devices.
     */
    cl::Program
    createProgram( const vector<string> &sources,
                  const cl::Context &context,
                  const vector<cl::Device> &devices,
                  const string &compileOptions = string() ) const;

    cl::Program
    createProgram( const vector<string> &sources,
                  const cl::Context &context,
                  const cl::Device &device,
                  const string &compileOptions = string() ) const;

    cl::Program
    createProgram( const string &source,
                  const cl::Context &context,
                  const vector<cl::Device> &devices,
                  const string &compileOptions = string() ) const;

    cl::Program
    createProgram( const string &source,
                  const cl::Context &context,
                  const cl::Device &device,
                  const string &compileOptions = string() ) const;

    /**
     *	\brief	Returns a kernel built from a program.
     */
    cl::Kernel
    createKernel(const string &programName,
                 const cl::Program &program) const;

    /**
     *	\brief	Returns a kernel vector built from a program.
     */
    vector<cl::Kernel>
    createKernels(cl::Program &program) const;

    /**
     *	\brief	Returns a map of function names with corresponding kernel
     *	        built from a program.
     */
    map<string, cl::Kernel>
    createKernelsMap(cl::Program &program) const;

    /**
     *	\brief	Returns source from a file read in.
     */
    string
    readSource(const string &filename) const;

    /**
     *	\brief	Returns buildlog from devices.
     */
    string
    getBuildLog(const cl::Program &program,
                const cl::Device &device) const;

    /**
     *	\brief	Returns kernel source read in.
     */
    string
    getBuildLog(const cl::Program &program,
                const vector<cl::Device> &devices) const;

    /**
     *	\brief	Prints information about platforms.
     */
    void
    printPlatformInfo(const cl::Platform &platform) const;

    /**
     *	\brief	Prints information about devices.
     */
    void
    printDeviceInfo(const cl::Device &device) const;

};


// Prototypes for stream operators

/**
 *  \brief  Stream operator for platform.
 */
ostream &operator<<(ostream &os, const cl::Platform &platform);

/**
 *  \brief  Stream operator for device.
 */
ostream &operator<<(ostream &os, const cl::Device &device);

#endif // __CLSETUP_HPP
