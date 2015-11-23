#ifndef BOARD_HPP
#define BOARD_HPP

#include "CLheader.hpp"


#include <map>
#include <string>

#include "field.h"



class Board {
public:
	Board(uint width, uint height, uint localWorkGroupSize, uint numLocalGroups);
	Board(const Board& that) = delete;
	~Board();
	
	void resetData();
	void fillRandomly();
	void stepDeviceNaive();
	void stepHost();
	void print(bool bWithNeighbourBits);
	void debugPrintDeviceData(bool bWithNeighbourBits);
	void checkConsistency();
	void checkRelevantConsistency();
	
	void updateFieldsDevice();
	void broadcastNeighboursDevice();
	
	void updateFieldsHost();
	void broadcastNeighboursHost();
	
	void uploadToDevice();
	void downloadFromDevice();
	
	void stepDeviceOptimized();
	
private:
	void init();
	void initCl(uint localWorkGroupSize, uint numLocalGroups);
	
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
};


#endif // BOARD_HPP
