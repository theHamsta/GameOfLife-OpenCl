#include <board.hpp>
#include <stdio.h>
#include <assert.h>

#define BOARD_PADDING_X (1)
#define BOARD_PADDING_Y (1)
#define BOARD_LINE_SKIP (this->m_widthDiv4 + 2 * BOARD_PADDING_X)
#define BOARD_PTR_FIRST_FIELD (this->m_dataHost + BOARD_PADDING_Y * BOARD_LINE_SKIP + BOARD_PADDING_X)

#define BOARD_GET_FIELD_PTR(X,Y) (&(this->m_dataHost[ ((X) + BOARD_PADDING_X) + ((Y) + BOARD_PADDING_Y) * BOARD_LINE_SKIP ]))

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

}


void Board::print()
{
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
	
	m_bDataValidDevice = true;
}

void Board::downloadFromDevice()
{
	assert( m_bDataValidDevice );
	
	m_bDataValidHost = true;
}

