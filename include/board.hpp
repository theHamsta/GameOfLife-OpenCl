#ifndef BOARD_HPP
#define BOARD_HPP

#include "CLheader.hpp"
#include "field.h"

#include <map>
#include <string>





class Board {
public:
	Board(uint width, uint height);
	Board(const Board& that) = delete;
	~Board();
	
	void clear();
	void fillRandomly();
	void step();
	void stepHost();
	void print();
	
private:
	void init();
	void initCl();
	void uploadToDevice();
	void downloadFromDevice();
	
	void updateFields();
	void boardcastNeighbours();
	
	void updateFieldsHost();
	void broadcastNeighboursHost();
	
	size_t getBufferSizeData();
	
	
	uint m_widthDiv4;
	uint m_heightDiv3;

	bool m_bDataValidHost;
	bool m_bDataValidDevice;
	
	// Host data
	field_t* m_dataHost;
	
	// Device data
	cl::Buffer m_dataDevice;
	cl::Buffer m_lutDevice;
	
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