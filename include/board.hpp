#ifndef BOARD_HPP
#define BOARD_HPP

#include "CLheader.hpp"


#include <map>
#include <string>

#include "field.h"



class Board {
public:
	Board(uint width, uint height, uint localWorkGroupSize, uint numLocalGroups, bool bEnableProfiling);
	Board(const Board& that) = delete;
	~Board();
	
	void resetData();
	void fillRandomly();

	void stepHost();
	void print(bool bWithNeighbourBits);
	void debugPrintDeviceData(bool bWithNeighbourBits);
	void checkConsistency();
	void checkRelevantConsistency();
	
	
	void uploadToDevice();
	void downloadFromDevice();
	
	void stepDeviceOptimized();
	
	inline cl_ulong getTotalStepKernelTime() { return m_totalStepKernelTime; }
	inline cl_ulong getTotalFixKernelTime() { return m_totalFixKernelTime; }
	
private:

	
	void initCl(uint localWorkGroupSize, uint numLocalGroups);
	
	void updateFieldsHost();
	void broadcastNeighboursHost();
	
	void stepDeviceNaive();
	void updateFieldsDevice();
	void broadcastNeighboursDevice();
	
	size_t getDataBufferSize();
	
	void fixOverlappingRegionsDevice();
	
	
	uint m_widthDiv4;
	uint m_heightDiv3;

	bool m_bDataValidHost;
	bool m_bDataValidDevice;
	
	// Host data
	field_t* m_dataHost;
	
	// Device data
	cl::Buffer m_dataDevice;
	cl::Buffer m_lutDevice;
	cl::Buffer m_dataUpperOverlappingRegionsDevice;
	cl::Buffer m_dataLowerOverlappingRegionsDevice;
	
	// OpenCl stuff
    std::map<std::string, cl::Kernel> m_kernels; 
	cl::Context m_context; 
	cl::Device m_device;
	cl::CommandQueue m_queue;
	cl::Program m_program;
    cl::NDRange m_localRange;
    cl::NDRange m_globalRange;
	
	bool m_bEnableProfiling;

	cl_ulong m_totalStepKernelTime;
	cl_ulong m_totalFixKernelTime;
};


#endif // BOARD_HPP
