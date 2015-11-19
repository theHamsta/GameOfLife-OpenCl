#ifndef  __CLHEADER_HPP
#define  __CLHEADER_HPP

#define ZERO_OFFSET 0

#ifdef __OPENCL_VERSION__


#if defined(DEBUG_MODE)
#pragma OPENCL EXTENSION cl_amd_printf : enable
#endif // DEBUG_MODE

// Enable atomic functions
#if defined(cl_khr_global_int32_base_atomics) && (cl_khr_global_int32_extended_atomics)
#pragma OPENCL EXTENSION cl_khr_global_int32_base_atomics : enable
#pragma OPENCL EXTENSION cl_khr_global_int32_extended_atomics : enable
#else
#error "Device does not support atomic operations!"
#endif // cl_khr_global_int32_base_atomics && cl_khr_global_int32_extended_atomics

#if defined(USE_CL_DOUBLE_PRECISION)

#if defined(cl_khr_fp64)
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#elif defined(cl_amd_fp64)
#pragma OPENCL EXTENSION cl_amd_fp64 : enable
#else
#error "No double precision supported!"
#endif // cl_khr_fp64

typedef double float_type;
typedef double2 float_type2;
typedef double4 float_type4;
#else
typedef float float_type;
typedef float2 float_type2;
typedef float4 float_type4;
#endif // USE_CL_DOUBLE_PRECISION

#else

#define CL_USE_DEPRECATED_OPENCL_1_1_APIS
#define __CL_ENABLE_EXCEPTIONS

#include "cl.hpp"
// #if defined(__APPLE__) || defined(__MACOSX)
// #include <OpenCL/cl.hpp>
// #else
// #include <CL/cl.hpp>
// #endif // __APPLE__

// Constants
static const cl_int END_OF_CELLS_LIST = -1;
static const int BC_REFLECTION = 2;
static const int BC_PERIODIC = 3;

#if defined(USE_DOUBLE_PRECISION)
	typedef cl_double float_type;
	typedef cl_double2 float_type2;
	typedef cl_double4 float_type4;
#else
	typedef cl_float float_type;
	typedef cl_float2 float_type2;
	typedef cl_float4 float_type4;
#endif // USE_DOUBLE_PRECISION



	
#endif // __OPENCL_VERSION__


#endif // __CLHEADER_HPP
