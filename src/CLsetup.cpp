
#include "CLsetup.hpp"


using std::string;
using std::vector;
using std::ifstream;
using std::runtime_error;
using std::make_pair;
using std::cout;
using std::cerr;
using std::cin;
using std::istreambuf_iterator;
using std::ostream;
using std::endl;


SetupCL::SetupCL()
{

}

SetupCL::~SetupCL()
{

}


vector<cl::Platform>
SetupCL::getPlatforms(void) const
{
    vector<cl::Platform> platforms;

    // Get platforms available
    cl::Platform::get(&platforms);

    if ( platforms.empty() )
    {
        throw runtime_error("No platforms found.");
    }

    return platforms;
}

cl::Platform
SetupCL::selectPlatform(void) const
{
    vector<cl::Platform> platforms;
    size_t platformID = 0;

    platforms = this->getPlatforms();

    // Print platform info
    for (vector<cl::Platform>::const_iterator cit = platforms.begin();
            cit != platforms.end(); ++cit)
    {
        this->printPlatformInfo(*cit);
    }

    // Select platform
    if (platforms.size() > 1)
    {
        platformID = platforms.size();

        while ( platformID >= platforms.size() )
        {
            cout << "Select a valid platform: " << endl;
            cin >> platformID;
        }
    }
    else if ( platforms.empty() )
    {
        // Return empty platform
        return cl::Platform();
    }

    return platforms[platformID];
}

cl::Context
SetupCL::createContext(cl_context_properties properties[],
                        const cl_device_type deviceType) const
{
    return cl::Context(deviceType, properties);
}

cl::Context
SetupCL::createContext(const cl::Platform &platform,
                        const cl_device_type deviceType) const
{
    // Create context
    cl_context_properties properties[] = {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties) (platform)(),
        0
    };

    return cl::Context(deviceType, properties);
}

vector<cl::Device>
SetupCL::getDevices(const cl::Context &context) const
{
    vector<cl::Device> devices;

    // Get a vector of devices on this platform
    devices = context.getInfo<CL_CONTEXT_DEVICES>();

    if ( devices.empty() )
    {
        throw runtime_error("No devices found!");
    }

    return devices;
}

vector<cl::Device>
SetupCL::getDevices(const cl::Platform &platform,
                     const cl_device_type deviceType) const
{
    vector<cl::Device> devices;

    // Get a vector of devices on this platform
    platform.getDevices(deviceType, &devices);

    if ( devices.empty() )
    {
        throw runtime_error("No devices found!");
    }

    return devices;
}

cl::Device
SetupCL::selectDevice(const cl::Context &context) const
{
    vector<cl::Device> devices = this->getDevices(context);
    size_t deviceID = 0;

    // Print device info 
    for (vector<cl::Device>::const_iterator cit = devices.begin();
            cit != devices.end(); ++cit)
    {
        this->printDeviceInfo(*cit);
    }

    if (devices.size() > 1)
    {
        const size_t size = devices.size();
        deviceID = devices.size();

        while (deviceID >= size)
        {
            cout << "Select valid device to use: " << endl;
            cin >> deviceID;
        }
    }
    else if ( devices.empty() )
    {
        // Return empty device
        return cl::Device();
    }

    return devices[deviceID];
}

cl::Device
SetupCL::selectDevice(const cl::Platform &platform,
                       const cl_device_type deviceType) const
{
    vector<cl::Device> devices = this->getDevices(platform, deviceType);
    size_t deviceID = 0;

    // Print device info 
    for (vector<cl::Device>::const_iterator cit = devices.begin();
            cit != devices.end(); ++cit)
    {
        this->printDeviceInfo(*cit);
    }

    if (devices.size() > 1)
    {
        const size_t size = devices.size();
        deviceID = devices.size();

        while (deviceID >= size)
        {
            cout << "Select valid device to use: " << endl;
            cin >> deviceID;
        }
    }
    else if ( devices.empty() )
    {
        // Return empty device
        return cl::Device();
    }

    return devices[deviceID];
}

string
SetupCL::readSource(const string &filename) const
{
    ifstream ifs( filename.c_str() );

    if ( !ifs.is_open() )
    {
        throw runtime_error("Could not open File!");
    }

    string source = string( istreambuf_iterator<char>(ifs),
                     istreambuf_iterator<char>() );

    return source;
}

cl::Program
SetupCL::createProgram(const vector<string> &sources,
                        const cl::Context &context,
                        const vector<cl::Device> &devices,
                        const string &compileOptions) const
{
    cl::Program program;
    cl::Program::Sources programSource;

    for (vector<string>::const_iterator cit = sources.begin();
            cit != sources.end(); ++cit)
    {
        programSource.push_back( std::make_pair( cit->c_str(),
                                 cit->length() ) );
    }

    // Make program of the source code in the context
    program = cl::Program(context, programSource);

	try{
    // Build program for those devices
    program.build( devices, compileOptions.c_str() );
		
	} catch (cl::Error& err) {
		// hi, sorry. I have seen, that this class already has a getBuildLog() method.
		// But how can you use it, when this program throws an error and therefore no valid program?
		cerr << "Error during kernel build." << endl
		<< "Compiler output for " << devices[0].getInfo<CL_DEVICE_NAME>() << ":" << endl
		<< program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]) << endl; //use first device. it must exist
		throw err; //TODO: how react? exit(-1)?
	}
#if  defined(DEBUG_MODE) || !defined(NDEBUG)
		// show compiler log always in DEBUG_MODE
		cout << "Compiler output for " << devices[0].getInfo<CL_DEVICE_NAME>() << ":" << endl
		<< program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]) << endl
		<<"========================"	<< endl;
#endif
    return program;
}

cl::Program
SetupCL::createProgram(const vector<string> &sources,
                        const cl::Context &context,
                        const cl::Device &device,
                        const string &compileOptions) const
{
    vector<cl::Device> devices;

    devices.push_back(device);

    return this->createProgram(sources, context, devices, compileOptions);
}

cl::Program
SetupCL::createProgram(const string &source,
                        const cl::Context &context,
                        const vector<cl::Device> &devices,
                        const string &compileOptions) const
{
    vector<string> sources;

    sources.push_back(source);

    return this->createProgram(sources, context, devices, compileOptions);
}

cl::Program
SetupCL::createProgram(const string &source,
                        const cl::Context &context,
                        const cl::Device &device,
                        const string &compileOptions) const
{
    vector<cl::Device> devices;
    vector<string> sources;

    devices.push_back(device);
    sources.push_back(source);

    return this->createProgram(sources, context, devices, compileOptions);
}

cl::Kernel
SetupCL::createKernel(const string &programName,
                       const cl::Program &program) const
{
    return cl::Kernel( program, programName.c_str() );
}

vector<cl::Kernel>
SetupCL::createKernels(cl::Program &program) const
{
    vector<cl::Kernel> kernels;

    program.createKernels(&kernels);

    return kernels;
}

map<string, cl::Kernel>
SetupCL::createKernelsMap(cl::Program &program) const
{
    map<string, cl::Kernel> ret;
    vector<cl::Kernel> kernels;

    program.createKernels(&kernels);

    for (vector<cl::Kernel>::const_iterator cit = kernels.begin();
            cit != kernels.end(); ++cit)
    {
        string kernelName = cit->getInfo<CL_KERNEL_FUNCTION_NAME>();
		
		// Intel version appends random \0 on std string... wtf??
		if ( kernelName[kernelName.length() - 1 ] == '\0' ) {
			kernelName.pop_back();
		}
        ret[kernelName] = *cit;
    }

    return ret;
}

string
SetupCL::getBuildLog(const cl::Program &program,
                      const cl::Device &device) const
{
    string ret;

    program.getBuildInfo(device, CL_PROGRAM_BUILD_LOG, &ret);

    return ret;
}

string
SetupCL::getBuildLog(const cl::Program &program,
                      const vector<cl::Device> &devices) const
{
    string ret;

    for (vector<cl::Device>::const_iterator cit = devices.begin();
            cit != devices.end(); ++cit)
    {
        ret += this->getBuildLog(program, *cit);
    }

    return ret;
}

void
SetupCL::printPlatformInfo(const cl::Platform &platform) const
{
    cout << platform << endl;
}

void
SetupCL::printDeviceInfo(const cl::Device &device) const
{
    cout << device << endl;
}


// Overloaded operators
ostream& operator<<(ostream &os, const cl::Platform &platform)
{
    os << "CL_PLATFORM_VERSION    = "
       << platform.getInfo<CL_PLATFORM_VERSION>() << endl;
    os << "CL_PLATFORM_NAME       = "
       << platform.getInfo<CL_PLATFORM_NAME>() << endl;
    os << "CL_PLATFORM_VENDOR     = "
       << platform.getInfo<CL_PLATFORM_VENDOR>() << endl;
    os << "CL_PLATFORM_EXTENSIONS = "
       << platform.getInfo<CL_PLATFORM_EXTENSIONS>() << endl;

    return os;
}

ostream& operator<<(ostream &os, const cl::Device &device)
{
    os << "CL_DEVICE_EXTENSIONS                    = "
       << device.getInfo<CL_DEVICE_EXTENSIONS>() << endl;
    os << "CL_DEVICE_GLOBAL_MEM_SIZE               = "
       << device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>() << endl;
    os << "CL_DEVICE_LOCAL_MEM_SIZE                = "
       << device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>() << endl;
    os << "CL_DEVICE_MAX_CLOCK_FREQUENCY           = "
       << device.getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>() << endl;
    os << "CL_DEVICE_MAX_COMPUTE_UNITS             = "
       << device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << endl;
    os << "CL_DEVICE_MAX_CONSTANT_ARGS             = "
       << device.getInfo<CL_DEVICE_MAX_CONSTANT_ARGS>() << endl;
    os << "CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE      = "
       << device.getInfo<CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE>() << endl;
    os << "CL_DEVICE_MAX_MEM_ALLOC_SIZE            = "
       << device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>() << endl;
    os << "CL_DEVICE_MAX_PARAMETER_SIZE            = "
       << device.getInfo<CL_DEVICE_MAX_PARAMETER_SIZE>() << endl;
    os << "CL_DEVICE_MAX_WORK_GROUP_SIZE           = "
       << device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>() << endl;
    os << "CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS      = "
       << device.getInfo<CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS>() << endl;

    vector<size_t> vecSizes = device.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();

    os << "CL_DEVICE_MAX_WORK_ITEM_SIZES           = ";
    os << "[";

    for (unsigned int i = 0; i < vecSizes.size(); i++)
    {
        os << vecSizes.at(i);

        if (i < vecSizes.size() - 1)
        {
            os << ", ";
        }
    }

    os << "]" << endl;

    os << "CL_DEVICE_NAME                          = "
       << device.getInfo<CL_DEVICE_NAME>() << endl;
    os << "CL_DEVICE_VENDOR                        = "
       << device.getInfo<CL_DEVICE_VENDOR>() << endl;
    os << "CL_DEVICE_VERSION                       = "
       << device.getInfo<CL_DEVICE_VERSION>() << endl;
    os << "CL_DRIVER_VERSION                       = "
       << device.getInfo<CL_DRIVER_VERSION>() << endl;
    os << "CL_DEVICE_PROFILE                       = "
       << device.getInfo<CL_DEVICE_PROFILE>() << endl;

    return os;
}


