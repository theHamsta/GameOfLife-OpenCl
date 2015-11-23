#include <board.hpp>
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <sstream>

#include "CLsetup.hpp"
#include "opencl_errorcodes.hpp"



#define BOARD_PADDING_X (1)
#define BOARD_PADDING_Y (1)
#define BOARD_LINE_SKIP (this->m_widthDiv4 + 2 * BOARD_PADDING_X)
#define BOARD_PTR_FIRST_FIELD (this->m_dataHost + BOARD_PADDING_Y * BOARD_LINE_SKIP + BOARD_PADDING_X)

#define BOARD_GET_FIELD_PTR(X,Y) (&(this->m_dataHost[ ((X) + BOARD_PADDING_X) + ((Y) + BOARD_PADDING_Y) * BOARD_LINE_SKIP ]))
#define BOARD_GET_FIELD_PTR_FROM_DATA(data, X,Y) (data +((X) + BOARD_PADDING_X) + ((Y) + BOARD_PADDING_Y) * BOARD_LINE_SKIP )


using namespace std;

Board::Board(uint widthDiv4, uint heightDiv3, uint localWorkGroupSize, uint numLocalGroups):
	m_widthDiv4(widthDiv4),
	m_heightDiv3(heightDiv3)
{
	m_dataHost = new field_t[ this->getDataBufferSize() / sizeof(field_t) ];
	
	this->resetData();
	this->initCl(localWorkGroupSize, numLocalGroups);
}

Board::~Board()
{
	delete m_dataHost;
}


void Board::resetData()
{
	memset(m_dataHost, 0, this->getDataBufferSize());

	m_bDataValidHost = true;
	m_bDataValidDevice = false;
}


void Board::initCl(uint localWorkGroupSize, uint numLocalGroups)
{
	// Read kernel sources
    const SetupCL clSetup;
	
    const string header = clSetup.readSource("include/CLheader.hpp");
	const string field_h = clSetup.readSource("include/field.h");
    vector<string> kernelSources;
    string source;

    source = clSetup.readSource("kernels/updateFields.cl");
	kernelSources.push_back(header + field_h + source);
	
	source = clSetup.readSource("kernels/broadcastNeighbourhoods.cl");
    kernelSources.push_back(header + field_h + source);
	
	source = clSetup.readSource("kernels/step.cl");
    kernelSources.push_back(header + field_h + source);
   
	source = clSetup.readSource("kernels/fixOverlappingRegions.cl");
    kernelSources.push_back(header + field_h + source);

    // Setup OpenCL
#if defined(DEBUG_MODE)
    cl::Platform platform = clSetup.selectPlatform();

    m_context = clSetup.createContext(platform);
    m_device = clSetup.selectDevice(m_context);
#else
    cl::Platform platform = clSetup.getPlatforms()[0];

    mContext = clSetup.createContext(platform);
    mDevice = clSetup.getDevices(mContext)[0];
#endif // DEBUG_MODE
	
	
    // Add compile time flags to opencl program
    stringstream compileTimeFlags;
	
#if defined(USE_DOUBLE_PRECISION)
    compileTimeFlags << " -DUSE_DOUBLE_PRECISION ";
#endif // USE_DOUBLE_PRECISION

#if defined(DEBUG_MODE)
    compileTimeFlags << " -DDEBUG_MODE ";
#endif // DEBUG_MODE

	// Vendor specific compiler options
	if (m_device.getInfo<CL_DEVICE_VENDOR>() == "NVIDIA Corporation") 
    {
        // the last two aren't NVIDIA-specific
		compileTimeFlags << " -DNVIDIA -cl-nv-verbose -cl-fast-relaxed-math -cl-mad-enable"; 
	}
	
	compileTimeFlags << " -DBOARD_WIDTH=" << m_widthDiv4 
					<< " -DBOARD_HEIGHT=" << m_heightDiv3
					<< " -DBOARD_PADDING_X=" << BOARD_PADDING_X
					<< " -DBOARD_PADDING_Y=" << BOARD_PADDING_Y;
	
					const char *compileTimeFlagsCstr = compileTimeFlags.str().c_str();

    cl::Program program = clSetup.createProgram(kernelSources, m_context,
                                    m_device, compileTimeFlagsCstr);

    m_kernels = clSetup.createKernelsMap(program);
	
	
    // Init queue
    m_queue = cl::CommandQueue(m_context, m_device);

	m_localRange = cl::NullRange;
    m_globalRange = cl::NullRange;
	


	// work group sizes
	
	
	
	
	m_localRange = cl::NDRange(localWorkGroupSize);// m_kernels.begin()->second.getWorkGroupInfo<CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE>(m_device);

// 	m_globalRange = cl::NDRange( ((m_widthDiv4 / localSize) + 1) * localSize );
	m_globalRange = cl::NDRange(  localWorkGroupSize * numLocalGroups );
	
	
	
	m_dataDevice = cl::Buffer(m_context, CL_MEM_READ_WRITE, this->getDataBufferSize());
	m_dataUpperOverlappingRegionsDevice = cl::Buffer(m_context, CL_MEM_READ_WRITE, (m_globalRange[0] / m_localRange[0]) * (m_widthDiv4 ) * sizeof(field_t));
	m_dataLowerOverlappingRegionsDevice = cl::Buffer(m_context, CL_MEM_READ_WRITE, (m_globalRange[0] / m_localRange[0]) * (m_widthDiv4 ) * sizeof(field_t));
	m_lutDevice = cl::Buffer(m_context, CL_MEM_READ_ONLY, FIELD_3X6LINE_LUT_SIZE);
}


void Board::print(bool bWithNeighbourBits)
{
	assert( m_bDataValidDevice || m_bDataValidHost );
	
// 	if( !m_bDataValidHost ) {
// 		this->downloadFromDevice();
// 	}
	
	for ( unsigned int y = 0; y < m_heightDiv3; y++ ) {
		for ( int l = BACTERIA_PER_FIELD_Y  + (bWithNeighbourBits ? +1 : - 1); l >= 0; l-- ) {
			for ( unsigned int x = 0; x < m_widthDiv4; x++ ) {

				if(bWithNeighbourBits) {
					field_printDebug(m_dataHost + (x+1) + (y+1) * (m_widthDiv4 + 2), l);
				} else {
					field_print(m_dataHost+ (x+1) + (y+1) * (m_widthDiv4 + 2), l);
				}

			}
			
			printf("\n");
		}

	}
	printf("\n");
}

void Board::debugPrintDeviceData(bool bWithNeighbourBits)
{
	field_t* controllBuffer = new field_t[getDataBufferSize() / sizeof(field_t)];
	
	m_queue.enqueueReadBuffer( m_dataDevice, CL_TRUE,
                               ZERO_OFFSET, getDataBufferSize(), controllBuffer );
	
	for ( unsigned int y = 0; y < m_heightDiv3; y++ ) {
		for ( int l = BACTERIA_PER_FIELD_Y + (bWithNeighbourBits ? +1 : - 1); l >= 0; l-- ) {
			for ( unsigned int x = 0; x < m_widthDiv4; x++ ) {
				
				if(bWithNeighbourBits) {
					field_printDebug(controllBuffer + (x+1) + (y+1) * (m_widthDiv4 + 2), l);
				} else {
					field_print(controllBuffer + (x+1) + (y+1) * (m_widthDiv4 + 2), l);
				}
			}
			
			printf("\n");
		}

	}
	printf("\n");
	
	delete controllBuffer;
}


void Board::stepHost()
{
	this->updateFieldsHost();
	this->broadcastNeighboursHost();
	
	m_bDataValidHost = true;
	m_bDataValidDevice = false;
}

void Board::stepDeviceNaive()
{
	this->updateFieldsDevice();
	this->broadcastNeighboursDevice();
	
	m_bDataValidHost = false;
	m_bDataValidDevice = true;
}

void Board::stepDeviceOptimized()
{
	cl::Kernel &kernel = m_kernels[ "stepDevice" ];
	try {
		uint blockHeight = (m_heightDiv3 - 1) / (m_globalRange[0] / m_localRange[0]) + 1;
		
		kernel.setArg( 0, m_dataDevice );
		kernel.setArg( 1, m_dataUpperOverlappingRegionsDevice);
		kernel.setArg( 2, m_dataLowerOverlappingRegionsDevice);
		kernel.setArg( 3, (m_localRange[0] + 2) * 3 * sizeof(field_t), NULL);
		kernel.setArg( 4, (blockHeight + 2) * sizeof(field_t) , NULL);

		m_queue.enqueueNDRangeKernel(kernel, ZERO_OFFSET_RANGE,
									m_globalRange, m_localRange );

		fixOverlappingRegionsDevice();
		m_queue.enqueueBarrier();
	} catch (cl::Error& err ) {
		cout << "Failed to call kernel stepDevice." << endl;
		cout << "Error \"" << getOpenClErrorString(err.err()) << "\"";
		cout << " in Function \"" << err.what() << "\"" <<  endl;
		exit(EXIT_FAILURE);
	}
}

void Board::fixOverlappingRegionsDevice()
{
	cl::Kernel &kernel = m_kernels[ "fixOverlappingRegions" ];
	try {
		kernel.setArg( 0, m_dataDevice );
		kernel.setArg( 1, m_dataUpperOverlappingRegionsDevice);
		kernel.setArg( 2, m_dataLowerOverlappingRegionsDevice);

		m_queue.enqueueNDRangeKernel(kernel, ZERO_OFFSET_RANGE,
									m_globalRange, m_localRange );

	} catch (cl::Error& err ) {
		cout << "Failed to call kernel fixOverlappingRegions." << endl;
		cout << "Error \"" << getOpenClErrorString(err.err()) << "\"";
		cout << " in Function \"" << err.what() << "\"" <<  endl;
		exit(EXIT_FAILURE);
	}
}



void Board::broadcastNeighboursHost()
{
	field_t* above = m_dataHost + BOARD_PADDING_X;
	field_t* current = BOARD_PTR_FIRST_FIELD;
	field_t* below = current + BOARD_LINE_SKIP;
	
	
	for ( unsigned int y = 0; y < m_heightDiv3; y++ ) {
		for( unsigned int x = 0; x < m_widthDiv4; x++ ) {
			
			
			if(FIELD_WAS_CHANGED(*current)) {
				field_broadcastLeft			(current, current - 1);
				field_broadcastTopLeft		(current, above - 1);
				field_broadcastTop			(current, above);
				field_broadcastTopRight		(current, above + 1);
				field_broadcastRight		(current, current + 1);
				field_broadcastBottomRight	(current, below + 1);
				field_broadcastBottom		(current, below);
				field_broadcastBottomLeft	(current, below - 1);
				
				FIELD_UNSET_WAS_CHANGED(*current);
			}
			
			
			
			above++;
			current++;
			below++;
		}
		above += 2 * BOARD_PADDING_X;
		current += 2 * BOARD_PADDING_X;
		below += 2 * BOARD_PADDING_X;
	}	
}

void Board::updateFieldsHost()
{
	for ( unsigned int y = 0; y < m_heightDiv3; y++ ) {
		for ( unsigned int x = 0; x < m_widthDiv4; x++ ) {

			field_updateNeighbourCount(BOARD_GET_FIELD_PTR(x,y));

		}
	}
}

void Board::updateFieldsDevice()
{
// 	if( !m_bDataValidDevice ) {
// 		this->uploadToDevice();
// 	}
	
	cl::Kernel &kernel = m_kernels[ "updateFieldsDevice" ];
	try {
		kernel.setArg( 0, m_dataDevice );

		m_queue.enqueueNDRangeKernel(kernel, ZERO_OFFSET_RANGE,
									m_globalRange, m_localRange );
		m_queue.enqueueBarrier();
	} catch (cl::Error& err ) {
		cout << "Failed to call kernel updateFieldsDevice." << endl;
		cout << "Error \"" << getOpenClErrorString(err.err()) << "\"";
		cout << " in Function \"" << err.what() << "\"" <<  endl;
		exit(EXIT_FAILURE);
	}
}

void Board::broadcastNeighboursDevice()
{
	cl::Kernel &kernel = m_kernels[ "broadcastNeighbourhoodsDevice" ];
	try {
		kernel.setArg( 0, m_dataDevice );

		m_queue.enqueueNDRangeKernel(kernel, ZERO_OFFSET_RANGE,
									m_globalRange, m_localRange );
		m_queue.enqueueBarrier();
	} catch (cl::Error& err ) {
		cout << "Failed to call kernel broadcastNeighbourhoodsDevice." << endl;
		cout << "Error \"" << getOpenClErrorString(err.err()) << "\"";
		cout << " in Function \"" << err.what() << "\"" <<  endl;
		exit(EXIT_FAILURE);
	}
}



void Board::fillRandomly()
{
	field_t* above = m_dataHost + BOARD_PADDING_X;
	field_t* current = BOARD_PTR_FIRST_FIELD;
	field_t* below = current + BOARD_LINE_SKIP;
	
	for ( unsigned int y = 0; y < m_heightDiv3; y++ ) {
		for ( unsigned int x = 0; x < m_widthDiv4; x++ ) {
			int rnd = rand();

			current->val |= rnd & FIELD_ALL_ELEMENTS_MASK;
			
			field_broadcastLeft			(current, current -1);
			field_broadcastTopLeft		(current, above - 1);
			field_broadcastTop			(current, above);
			field_broadcastTopRight		(current, above + 1);
			field_broadcastRight		(current, current + 1);
			field_broadcastBottomRight	(current, below + 1);
			field_broadcastBottom		(current, below);
			field_broadcastBottomLeft	(current, below - 1);
			
			above++;
			current++;
			below++;
			
			FIELD_UNSET_WAS_CHANGED(*current);
		}
		above += 2 * BOARD_PADDING_X;
		current += 2 * BOARD_PADDING_X;
		below += 2 * BOARD_PADDING_X;
	}
	
	m_bDataValidHost = true;
	m_bDataValidDevice = false;
}




size_t Board::getDataBufferSize()
{
	return sizeof(field_t)
		* (m_heightDiv3 + 2 * BOARD_PADDING_Y)
		* (m_widthDiv4 + 2 * BOARD_PADDING_X);
}

void Board::uploadToDevice()
{
	assert( m_bDataValidHost );
	
	m_queue.enqueueWriteBuffer( m_dataDevice, CL_TRUE,
                               ZERO_OFFSET, getDataBufferSize(), m_dataHost );
	
	m_bDataValidDevice = true;
}

void Board::downloadFromDevice()
{
	assert( m_bDataValidDevice );
	
	m_queue.enqueueReadBuffer( m_dataDevice, CL_TRUE,
                               ZERO_OFFSET, getDataBufferSize(), m_dataHost );
	
	m_bDataValidHost = true;
}

void Board::checkConsistency()
{
	field_t* controllBuffer = new field_t[getDataBufferSize() / sizeof(field_t)];
	
	m_queue.enqueueReadBuffer( m_dataDevice, CL_TRUE,
                               ZERO_OFFSET, getDataBufferSize(), controllBuffer );
	
	for( uint i = 0; i < getDataBufferSize() / sizeof(field_t); i++ ){
		assert(controllBuffer[i].val == m_dataHost[i].val);
	}
	
	delete controllBuffer;
}

void Board::checkRelevantConsistency()
{
	field_t* controllBuffer = new field_t[getDataBufferSize() / sizeof(field_t)];

	m_queue.enqueueReadBuffer( m_dataDevice, CL_TRUE,
                               ZERO_OFFSET, getDataBufferSize(), controllBuffer );

	for (uint y = 0; y < m_heightDiv3; y++){
		for ( uint x = 0; x < m_widthDiv4; x++){
			if( BOARD_GET_FIELD_PTR(x,y)->val != BOARD_GET_FIELD_PTR_FROM_DATA(controllBuffer, x, y)->val){
				printf("%i,%i\n",x,y);
				field_printDebugAllLines(BOARD_GET_FIELD_PTR(x,y));
				field_printDebugAllLines(BOARD_GET_FIELD_PTR_FROM_DATA(controllBuffer, x, y));

				exit(-1);
			}
		}
	}

	delete controllBuffer;
}


