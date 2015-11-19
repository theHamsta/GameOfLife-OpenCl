#include "CLheader.hpp"
#include "opencl_errorcodes.hpp"


std::string getOpenClErrorString( int errorCode ) {
    
    if(errorCode == CL_SUCCESS)
        return "CL_SUCCESS";      								// 0
    else if(errorCode == CL_DEVICE_NOT_FOUND)
        return "CL_DEVICE_NOT_FOUND"; 							// -1
    else if(errorCode == CL_DEVICE_NOT_AVAILABLE)
        return "CL_DEVICE_NOT_AVAILABLE"; 						// -2
    else if(errorCode == CL_COMPILER_NOT_AVAILABLE)
        return "CL_COMPILER_NOT_AVAILABLE";      				// -3
    else if(errorCode == CL_MEM_OBJECT_ALLOCATION_FAILURE)
        return "CL_MEM_OBJECT_ALLOCATION_FAILURE"; 				// -4
    else if(errorCode == CL_OUT_OF_RESOURCES)
        return "CL_OUT_OF_RESOURCES"; 							// -5
    else if(errorCode == CL_OUT_OF_HOST_MEMORY)
        return "CL_OUT_OF_HOST_MEMORY";      					// -6
    else if(errorCode == CL_PROFILING_INFO_NOT_AVAILABLE)
        return "CL_PROFILING_INFO_NOT_AVAILABLE"; 				// -7
    else if(errorCode == CL_MEM_COPY_OVERLAP)
        return "CL_MEM_COPY_OVERLAP"; 							// -8
    else if(errorCode == CL_IMAGE_FORMAT_MISMATCH)
        return "CL_IMAGE_FORMAT_MISMATCH";      				// -9
    else if(errorCode == CL_IMAGE_FORMAT_NOT_SUPPORTED)
        return "CL_IMAGE_FORMAT_NOT_SUPPORTED"; 				// -10
    else if(errorCode == CL_BUILD_PROGRAM_FAILURE)
        return "CL_BUILD_PROGRAM_FAILURE"; 						// -11
    else if(errorCode == CL_MAP_FAILURE)
        return "CL_MAP_FAILURE";      							// -12
    else if(errorCode == CL_MISALIGNED_SUB_BUFFER_OFFSET)
        return "CL_MISALIGNED_SUB_BUFFER_OFFSET"; 				// -13
    else if(errorCode == CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST)
        return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST"; 	// -14
    else if(errorCode == CL_BUILD_PROGRAM_FAILURE)
        return "CL_BUILD_PROGRAM_FAILURE"; 						// -15
    else if(errorCode == CL_LINKER_NOT_AVAILABLE)
        return "CL_LINKER_NOT_AVAILABLE";      					// -16
    else if(errorCode == CL_LINK_PROGRAM_FAILURE)
        return "CL_LINK_PROGRAM_FAILURE"; 						// -17
    else if(errorCode == CL_DEVICE_PARTITION_FAILED)
        return "CL_DEVICE_PARTITION_FAILED"; 					// -18
    else if(errorCode == CL_KERNEL_ARG_INFO_NOT_AVAILABLE)
        return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE"; 				// -19
		
    else if(errorCode == CL_INVALID_VALUE)                      // -30
        return "CL_INVALID_VALUE";
    else if(errorCode == CL_INVALID_DEVICE_TYPE)                // -31
        return "CL_INVALID_DEVICE_TYPE";
    else if(errorCode == CL_INVALID_PLATFORM)                   // -32
        return "CL_INVALID_PLATFORM";
    else if(errorCode == CL_INVALID_DEVICE)                     // -33
        return "CL_INVALID_DEVICE";
    else if(errorCode == CL_INVALID_CONTEXT)                    // -34
        return "CL_INVALID_CONTEXT";
    else if(errorCode == CL_INVALID_QUEUE_PROPERTIES)           // -35
        return "CL_INVALID_QUEUE_PROPERTIES";
    else if(errorCode == CL_INVALID_COMMAND_QUEUE)              // -36
        return "CL_INVALID_COMMAND_QUEUE";
    else if(errorCode == CL_INVALID_HOST_PTR)                   // -37
        return "CL_INVALID_HOST_PTR";
    else if(errorCode == CL_INVALID_MEM_OBJECT)                 // -38
        return "CL_INVALID_MEM_OBJECT";
    else if(errorCode == CL_INVALID_IMAGE_FORMAT_DESCRIPTOR)    // -39
        return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
    else if(errorCode == CL_INVALID_IMAGE_SIZE)                 // -40
        return "CL_INVALID_IMAGE_SIZE";
    else if(errorCode == CL_INVALID_SAMPLER)                    // -41
        return "CL_INVALID_SAMPLER";
    else if(errorCode == CL_INVALID_BINARY)                     // -42
        return "CL_INVALID_BINARY";
    else if(errorCode == CL_INVALID_BUILD_OPTIONS)              // -43
        return "CL_INVALID_BUILD_OPTIONS";
    else if(errorCode == CL_INVALID_PROGRAM)                    // -44
        return "CL_INVALID_PROGRAM";
    else if(errorCode == CL_INVALID_PROGRAM_EXECUTABLE)         // -45
        return "CL_INVALID_PROGRAM_EXECUTABLE";
    else if(errorCode == CL_INVALID_KERNEL_NAME)                // -46
        return "CL_INVALID_KERNEL_NAME";
    else if(errorCode == CL_INVALID_KERNEL_DEFINITION)          // -47
        return "CL_INVALID_KERNEL_DEFINITION";
    else if(errorCode == CL_INVALID_KERNEL)                     // -48
        return "CL_INVALID_KERNEL";
    else if(errorCode == CL_INVALID_ARG_INDEX)                  // -49
        return "CL_INVALID_ARG_INDEX";
    else if(errorCode == CL_INVALID_ARG_VALUE)                  // -50
        return "CL_INVALID_ARG_VALUE";
    else if(errorCode == CL_INVALID_ARG_SIZE)                   // -51
        return "CL_INVALID_ARG_SIZE";
    else if(errorCode == CL_INVALID_KERNEL_ARGS)                // -52
        return "CL_INVALID_KERNEL_ARGS";
    else if(errorCode == CL_INVALID_WORK_DIMENSION)             // -53
        return "CL_INVALID_WORK_DIMENSION";
    else if(errorCode == CL_INVALID_WORK_GROUP_SIZE)            // -54
        return "CL_INVALID_WORK_GROUP_SIZE";
    else if(errorCode == CL_INVALID_WORK_ITEM_SIZE)             // -55
        return "CL_INVALID_WORK_ITEM_SIZE";
    else if(errorCode == CL_INVALID_GLOBAL_OFFSET)              // -56
        return "CL_INVALID_GLOBAL_OFFSET";
    else if(errorCode == CL_INVALID_EVENT_WAIT_LIST)            // -57
        return "CL_INVALID_EVENT_WAIT_LIST";
    else if(errorCode == CL_INVALID_EVENT)                      // -58
        return "CL_INVALID_EVENT";
    else if(errorCode == CL_INVALID_OPERATION)                  // -59
        return "CL_INVALID_OPERATION";
    else if(errorCode == CL_INVALID_GL_OBJECT)                  // -60
        return "CL_INVALID_GL_OBJECT";
    else if(errorCode == CL_INVALID_BUFFER_SIZE)                // -61
        return "CL_INVALID_BUFFER_SIZE";
    else if(errorCode == CL_INVALID_MIP_LEVEL)                  // -62
        return "CL_INVALID_MIP_LEVEL";
    else if(errorCode == CL_INVALID_GLOBAL_WORK_SIZE)           // -63
        return "CL_INVALID_GLOBAL_WORK_SIZE";
    else if(errorCode == CL_INVALID_PROPERTY)                   // -64
        return "CL_INVALID_PROPERTY";
    else if(errorCode == CL_INVALID_IMAGE_DESCRIPTOR)           // -65
        return "CL_INVALID_IMAGE_DESCRIPTOR";
    else if(errorCode == CL_INVALID_COMPILER_OPTIONS)           // -66
        return "CL_INVALID_COMPILER_OPTIONS";
    else if(errorCode == CL_INVALID_LINKER_OPTIONS)             // -67
        return "CL_INVALID_LINKER_OPTIONS";
    else if(errorCode == CL_INVALID_DEVICE_PARTITION_COUNT)     // -68
        return "CL_INVALID_DEVICE_PARTITION_COUNT";
    else
        return "Unknown Error";
}

