#ifndef OPENCL_ERRORCODES_HPP
#define OPENCL_ERRORCODES_HPP



#include <string>
#include "CLheader.hpp"


/* Error Codes */
#define CL_SUCCESS                                  0
#define CL_DEVICE_NOT_FOUND                         -1
#define CL_DEVICE_NOT_AVAILABLE                     -2
#define CL_COMPILER_NOT_AVAILABLE                   -3
#define CL_MEM_OBJECT_ALLOCATION_FAILURE            -4
#define CL_OUT_OF_RESOURCES                         -5
#define CL_OUT_OF_HOST_MEMORY                       -6
#define CL_PROFILING_INFO_NOT_AVAILABLE             -7
#define CL_MEM_COPY_OVERLAP                         -8
#define CL_IMAGE_FORMAT_MISMATCH                    -9
#define CL_IMAGE_FORMAT_NOT_SUPPORTED               -10
#define CL_BUILD_PROGRAM_FAILURE                    -11
#define CL_MAP_FAILURE                              -12
#define CL_MISALIGNED_SUB_BUFFER_OFFSET             -13
#define CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST    -14

#ifndef CL_COMPILE_PROGRAM_FAILURE
#define CL_COMPILE_PROGRAM_FAILURE                  -15
#endif // CL_COMPILE_PROGRAM_FAILURE
#ifndef CL_LINKER_NOT_AVAILABLE
#define CL_LINKER_NOT_AVAILABLE                     -16
#endif // CL_LINKER_NOT_AVAILABLE
#ifndef CL_LINK_PROGRAM_FAILURE
#define CL_LINK_PROGRAM_FAILURE                     -17
#endif // CL_LINK_PROGRAM_FAILURE
#ifndef CL_DEVICE_PARTITION_FAILED
#define CL_DEVICE_PARTITION_FAILED                  -18
#endif // CL_DEVICE_PARTITION_FAILED
#ifndef CL_KERNEL_ARG_INFO_NOT_AVAILABLE
#define CL_KERNEL_ARG_INFO_NOT_AVAILABLE            -19
#endif // CL_KERNEL_ARG_INFO_NOT_AVAILABLE

#define CL_INVALID_VALUE                            -30
#define CL_INVALID_DEVICE_TYPE                      -31
#define CL_INVALID_PLATFORM                         -32
#define CL_INVALID_DEVICE                           -33
#define CL_INVALID_CONTEXT                          -34
#define CL_INVALID_QUEUE_PROPERTIES                 -35
#define CL_INVALID_COMMAND_QUEUE                    -36
#define CL_INVALID_HOST_PTR                         -37
#define CL_INVALID_MEM_OBJECT                       -38
#define CL_INVALID_IMAGE_FORMAT_DESCRIPTOR          -39
#define CL_INVALID_IMAGE_SIZE                       -40
#define CL_INVALID_SAMPLER                          -41
#define CL_INVALID_BINARY                           -42
#define CL_INVALID_BUILD_OPTIONS                    -43
#define CL_INVALID_PROGRAM                          -44
#define CL_INVALID_PROGRAM_EXECUTABLE               -45
#define CL_INVALID_KERNEL_NAME                      -46
#define CL_INVALID_KERNEL_DEFINITION                -47
#define CL_INVALID_KERNEL                           -48
#define CL_INVALID_ARG_INDEX                        -49
#define CL_INVALID_ARG_VALUE                        -50
#define CL_INVALID_ARG_SIZE                         -51
#define CL_INVALID_KERNEL_ARGS                      -52
#define CL_INVALID_WORK_DIMENSION                   -53
#define CL_INVALID_WORK_GROUP_SIZE                  -54
#define CL_INVALID_WORK_ITEM_SIZE                   -55
#define CL_INVALID_GLOBAL_OFFSET                    -56
#define CL_INVALID_EVENT_WAIT_LIST                  -57
#define CL_INVALID_EVENT                            -58
#define CL_INVALID_OPERATION                        -59
#define CL_INVALID_GL_OBJECT                        -60
#define CL_INVALID_BUFFER_SIZE                      -61
#define CL_INVALID_MIP_LEVEL                        -62
#define CL_INVALID_GLOBAL_WORK_SIZE                 -63
#define CL_INVALID_PROPERTY                         -64

#ifndef CL_INVALID_IMAGE_DESCRIPTOR
#define CL_INVALID_IMAGE_DESCRIPTOR                 -65
#endif // CL_INVALID_IMAGE_DESCRIPTOR
#ifndef CL_INVALID_COMPILER_OPTIONS
#define CL_INVALID_COMPILER_OPTIONS                 -66
#endif // CL_INVALID_COMPILER_OPTIONS
#ifndef CL_INVALID_LINKER_OPTIONS
#define CL_INVALID_LINKER_OPTIONS                   -67
#endif // CL_INVALID_LINKER_OPTIONS
#ifndef CL_INVALID_DEVICE_PARTITION_COUNT
#define CL_INVALID_DEVICE_PARTITION_COUNT           -68
#endif // CL_INVALID_DEVICE_PARTITION_COUNT

/**
 * @brief Function that converts OpenCL's cryptic error codes into their actual name
 * 
 * @param errorCode OpenCL error code
 * @return Type of error as string
 */
std::string getOpenClErrorString( int errorCode );

#endif
