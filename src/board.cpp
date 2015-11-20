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



using namespace std;

Board::Board(uint widthDiv4, uint heightDiv3):
	m_widthDiv4(widthDiv4),
	m_heightDiv3(heightDiv3)
{
	m_dataHost = new field_t[ this->getBufferSizeData() / sizeof(field_t) ];
	
	init();
}

Board::~Board()
{
	delete m_dataHost;
}


void Board::clear()
{
	memset(m_dataHost, 0, this->getBufferSizeData());

	m_bDataValidHost = true;
	m_bDataValidDevice = false;
}

void Board::init()
{
	this->initCl();
	this->clear();
}

void Board::initCl()
{
	// Read kernel sources
    const SetupCL clSetup;
	
    const string header = clSetup.readSource("include/CLheader.hpp");
	const string field_h = clSetup.readSource("include/field.h");
    vector<string> kernelSources;
    string source;

    source = clSetup.readSource("kernels/updateFields.cl");
	kernelSources.push_back(header + field_h + source);
	
	source = clSetup.readSource("kernels/broadcastNeighbours.cl");
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
		compileTimeFlags << " -cl-nv-verbose -cl-fast-relaxed-math -cl-mad-enable"; 
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
	
	
	m_dataDevice = cl::Buffer(m_context, CL_MEM_READ_WRITE, this->getBufferSizeData());
	m_lutDevice = cl::Buffer(m_context, CL_MEM_READ_ONLY, FIELD_3X6LINE_LUT_SIZE);

	// work group sizes
	
	uint localSize = m_kernels.begin()->second.getWorkGroupInfo<CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE>(m_device);
	
	
	m_localRange = cl::NDRange(
	localSize,
	1,
	1);
	
	m_globalRange = cl::NDRange( ((m_widthDiv4 / localSize) + 1) * localSize, 1, 1 );
	
	cout << "Local workgroup size: " << m_localRange[0] << endl;
	cout << "Global workgroup size: " << m_globalRange[0] << endl;
}


void Board::print()
{
	assert( m_bDataValidDevice || m_bDataValidHost );
	
	if( !m_bDataValidHost ) {
		this->downloadFromDevice();
	}
	
	for ( unsigned int y = 0; y < m_heightDiv3; y++ ) {
		for ( int l = BACTERIA_PER_FIELD_Y - 1; l >= 0; l-- ) {
			for ( unsigned int x = 0; x < m_widthDiv4; x++ ) {

				field_print(BOARD_GET_FIELD_PTR(x,y), l);

			}
			
			printf("\n");
		}

	}
	printf("\n");
}

void Board::debugPrintDeviceData()
{
	m_bDataValidDevice = true;
	m_bDataValidHost = false;
	this->print();
}


void Board::stepHost()
{
	this->updateFieldsHost();
	this->broadcastNeighboursHost();
	
	m_bDataValidHost = true;
	m_bDataValidDevice = false;
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
									m_localRange, m_globalRange);
		m_queue.enqueueBarrier();
	} catch (cl::Error& err ) {
		cout << "Failed to call kernel updateFieldsDevice." << endl;
		cout << "Error \"" << getOpenClErrorString(err.err()) << "\"";
		cout << " in Function \"" << err.what() << "\"" <<  endl;
		exit(EXIT_FAILURE);
	}
}

void Board::boardcastNeighboursDevice()
{
	cl::Kernel &kernel = m_kernels[ "broadcastNeighboursDevice" ];
	try {
		kernel.setArg( 0, m_dataDevice );

		m_queue.enqueueNDRangeKernel(kernel, ZERO_OFFSET_RANGE,
									m_localRange, m_globalRange);
		m_queue.enqueueBarrier();
	} catch (cl::Error& err ) {
		cout << "Failed to call kernel broadcastNeighboursDevice." << endl;
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
			
		}
		above += 2 * BOARD_PADDING_X;
		current += 2 * BOARD_PADDING_X;
		below += 2 * BOARD_PADDING_X;
	}
	
	m_bDataValidHost = true;
	m_bDataValidDevice = false;
}




size_t Board::getBufferSizeData()
{
	return sizeof(field_t)
		* (m_heightDiv3 + 2 * BOARD_PADDING_Y)
		* (m_widthDiv4 + 2 * BOARD_PADDING_X);
}

void Board::uploadToDevice()
{
	assert( m_bDataValidHost );
	
	m_queue.enqueueWriteBuffer( m_dataDevice, CL_TRUE,
                               ZERO_OFFSET, getBufferSizeData(), m_dataHost );
	
	m_bDataValidDevice = true;
}

void Board::downloadFromDevice()
{
	assert( m_bDataValidDevice );
	
	m_queue.enqueueReadBuffer( m_dataDevice, CL_TRUE,
                               ZERO_OFFSET, getBufferSizeData(), m_dataHost );
	
	m_bDataValidHost = true;
}

void Board::checkConsistency()
{
	field_t* controllBuffer = new field_t[getBufferSizeData() / sizeof(field_t)];
	
	m_queue.enqueueReadBuffer( m_dataDevice, CL_TRUE,
                               ZERO_OFFSET, getBufferSizeData(), controllBuffer );
	
	for( uint i = 0; i < getBufferSizeData() / sizeof(field_t); i++ ){
		assert(controllBuffer[i].val == m_dataHost[i].val);
	}
	
	delete controllBuffer;
}


