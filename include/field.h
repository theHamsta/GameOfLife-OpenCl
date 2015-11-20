#ifndef FIELD_H
#define FIELD_H




#define BACTERIA_PER_FIELD_X 4
#define BACTERIA_PER_FIELD_Y 3
#define BACTERIA_PER_FIELD   (BACTERIA_PER_FIELD_X * BACTERIA_PER_FIELD_Y)


#define FIELD_LINE_WIDTH  ((BACTERIA_PER_FIELD_X) + 2)
#define FIELD_ABOVE_MASK ( 0x7 << (2*FIELD_LINE_WIDTH))
#define FIELD_BELOW_MASK ( 0x7 ) 
#define FIELD_LEFT_AND_RIGHT_MASK ( 0x5 << (FIELD_LINE_WIDTH)) 
#define FIELD_NEIGHBOUR_MASK ( FIELD_ABOVE_MASK | FIELD_BELOW_MASK | FIELD_LEFT_AND_RIGHT_MASK )

#define FIELD_THREE_LINES_MASK ( (1<<(3*FIELD_LINE_WIDTH)) - 1 )

#define FIELD_LINE_WIDTH_ONES ((1 << BACTERIA_PER_FIELD_X) - 1)
#define FIELD_ALL_ELEMENTS_MASK ( (FIELD_LINE_WIDTH_ONES << FIELD_ELEMENT_SHIFT_FOR_MASK(1,1)) | (FIELD_LINE_WIDTH_ONES << FIELD_ELEMENT_SHIFT_FOR_MASK(1,2)) | (FIELD_LINE_WIDTH_ONES << FIELD_ELEMENT_SHIFT_FOR_MASK(1,3)))
#define FIELD_NON_ELEMENTS_MASK (~FIELD_ALL_ELEMENTS_MASK)

//#define FIELD_ELEMENT_SHIFT(X,Y) ( (1 + X) + (Y + 1) * FIELD_LINE_WIDTH )
#define FIELD_ELEMENT_SHIFT_FOR_MASK(X,Y) ( (X) + (Y) * FIELD_LINE_WIDTH )

#define FIELD_GET_ELEMENT(FIELD, X, Y) (((FIELD).val >> FIELD_ELEMENT_SHIFT_FOR_MASK((X+1),(Y+1))) & 1)


#define FIELD_CHANGED_BIT_POS 31
#define FIELD_CHANGED_MASK (1 << FIELD_CHANGED_BIT_POS)

#define FIELD_WAS_CHANGED(FIELD)	((FIELD).val & FIELD_CHANGED_MASK)
// #define FIELD_SET_WAS_CHANGED(FIELD) (FIELD_WAS_CHANGED(FIELD) != false)
#define FIELD_UNSET_WAS_CHANGED(FIELD) ((FIELD).val &=  ~FIELD_CHANGED_MASK)


// #define BACTERIA_PER_FIELD 4
// 
// #define FIELD_ABOVE_MASK (0x7)
// #define FIELD_BELOW_MASK (0x7 << 6)
// #define FIELD_LEFT_AND_RIGHT_MASK (0x5 << 12)
// 
// #define FIELD_NEIGHBOUR_MASK ( FIELD_ABOVE_MASK || FIELD_BELOW_MASK || FIELD_LEFT_AND_RIGHT_MASK );
// 
// #define FIELD_GET_ELEMENT(FIELD, INDEX) (((FIELD).val >> (INDEX)) & 1)



#define BROADCAST_LEFT (FIELD, NEIGHBOUR) 
#define BROADCAST_TOP_LEFT (FIELD, NEIGHBOUR) 
#define BROADCAST_TOP (FIELD, NEIGHBOUR) 
#define BROADCAST_TOP_RIGHT (FIELD, NEIGHBOUR) 
#define BROADCAST_RIGHT (FIELD, NEIGHBOUR) 
#define BROADCAST_BOTTOM_RIGHT (FIELD, NEIGHBOUR) 
#define BROADCAST_BOTTOM (FIELD, NEIGHBOUR) 
#define BROADCAST_BOTTOM_LEFT (FIELD, NEIGHBOUR) 

#define FIELD_ALL_NEIGHBOURS_LEFT_MASK ( (1 << FIELD_ELEMENT_SHIFT_FOR_MASK(BACTERIA_PER_FIELD_X + 1,1)) | (1 << FIELD_ELEMENT_SHIFT_FOR_MASK(BACTERIA_PER_FIELD_X + 1,2)) | (1 << FIELD_ELEMENT_SHIFT_FOR_MASK(BACTERIA_PER_FIELD_X + 1,3)))
#define FIELD_ALL_NEIGHBOURS_TOP_LEFT_MASK (1 << FIELD_ELEMENT_SHIFT_FOR_MASK(BACTERIA_PER_FIELD_X + 1, BACTERIA_PER_FIELD_Y + 1)) 
#define FIELD_ALL_NEIGHBOURS_TOP_MASK ( ((1<<(BACTERIA_PER_FIELD_X))-1) << FIELD_ELEMENT_SHIFT_FOR_MASK(1,BACTERIA_PER_FIELD_Y+1)) 
#define FIELD_ALL_NEIGHBOURS_TOP_RIGHT_MASK ( 1 << FIELD_ELEMENT_SHIFT_FOR_MASK(0,BACTERIA_PER_FIELD_Y+1)) 
#define FIELD_ALL_NEIGHBOURS_RIGHT_MASK ( (1 << FIELD_ELEMENT_SHIFT_FOR_MASK(0,1)) | (1 << FIELD_ELEMENT_SHIFT_FOR_MASK(0,2)) | (1 << FIELD_ELEMENT_SHIFT_FOR_MASK(0,3)))
#define FIELD_ALL_NEIGHBOURS_BOTTOM_RIGHT_MASK ( 1 << FIELD_ELEMENT_SHIFT_FOR_MASK(0,0)) 
#define FIELD_ALL_NEIGHBOURS_BOTTOM_MASK (((1<<(BACTERIA_PER_FIELD_X))-1) << FIELD_ELEMENT_SHIFT_FOR_MASK(1,0)) 
#define FIELD_ALL_NEIGHBOURS_BOTTOM_LEFT_MASK ( 1 << FIELD_ELEMENT_SHIFT_FOR_MASK(BACTERIA_PER_FIELD_X + 1,0))

#define FIELD_ALL_ELEMENTS_LEFT_MASK ( (1 << FIELD_ELEMENT_SHIFT_FOR_MASK(BACTERIA_PER_FIELD_X,1)) | (1 << FIELD_ELEMENT_SHIFT_FOR_MASK(BACTERIA_PER_FIELD_X,2)) | (1 << FIELD_ELEMENT_SHIFT_FOR_MASK(BACTERIA_PER_FIELD_X,3)))
#define FIELD_ALL_ELEMENTS_TOP_LEFT_MASK (1 << FIELD_ELEMENT_SHIFT_FOR_MASK(BACTERIA_PER_FIELD_X,BACTERIA_PER_FIELD_Y)) 
#define FIELD_ALL_ELEMENTS_TOP_MASK ( ((1<<(BACTERIA_PER_FIELD_X))-1) << FIELD_ELEMENT_SHIFT_FOR_MASK(1,BACTERIA_PER_FIELD_Y)) 
#define FIELD_ALL_ELEMENTS_TOP_RIGHT_MASK ( 1 << FIELD_ELEMENT_SHIFT_FOR_MASK(1,BACTERIA_PER_FIELD_Y)) 
#define FIELD_ALL_ELEMENTS_RIGHT_MASK ( (1 << FIELD_ELEMENT_SHIFT_FOR_MASK(1,1)) | (1 << FIELD_ELEMENT_SHIFT_FOR_MASK(1,2)) | (1 << FIELD_ELEMENT_SHIFT_FOR_MASK(1,3)))
#define FIELD_ALL_ELEMENTS_BOTTOM_RIGHT_MASK ( 1 << FIELD_ELEMENT_SHIFT_FOR_MASK(1,1)) 
#define FIELD_ALL_ELEMENTS_BOTTOM_MASK (((1<<(BACTERIA_PER_FIELD_X))-1) << FIELD_ELEMENT_SHIFT_FOR_MASK(1,1)) 
#define FIELD_ALL_ELEMENTS_BOTTOM_LEFT_MASK ( 1 << FIELD_ELEMENT_SHIFT_FOR_MASK(BACTERIA_PER_FIELD_X,1))

#define FIELD_ELEMENT_BL_POS FIELD_ELEMENT_SHIFT_FOR_MASK(BACTERIA_PER_FIELD_X,1)
#define FIELD_ELEMENT_BR_POS FIELD_ELEMENT_SHIFT_FOR_MASK(1,1)
#define FIELD_ELEMENT_TL_POS FIELD_ELEMENT_SHIFT_FOR_MASK(BACTERIA_PER_FIELD_X,BACTERIA_PER_FIELD_Y)
#define FIELD_ELEMENT_TR_POS FIELD_ELEMENT_SHIFT_FOR_MASK(1,BACTERIA_PER_FIELD_Y)

#define FIELD_NEIGHBOUR_BL_POS FIELD_ELEMENT_SHIFT_FOR_MASK(BACTERIA_PER_FIELD_X + 1,0)
#define FIELD_NEIGHBOUR_BR_POS FIELD_ELEMENT_SHIFT_FOR_MASK(0,0)
#define FIELD_NEIGHBOUR_TL_POS FIELD_ELEMENT_SHIFT_FOR_MASK(BACTERIA_PER_FIELD_X + 1, BACTERIA_PER_FIELD_Y + 1)
#define FIELD_NEIGHBOUR_TR_POS FIELD_ELEMENT_SHIFT_FOR_MASK(0,BACTERIA_PER_FIELD_Y + 1)



#ifdef __OPENCL_VERSION__
#define UINT32_MAX 0xFFFFFFFF
typedef uint uint32_t;
typedef struct { uint val; } field_t;
#else
#include <stdint.h>
struct field_s 
{
	unsigned int right0:1;
	unsigned int line0 :BACTERIA_PER_FIELD_X;
	unsigned int left0 :1;
	
	unsigned int right1:1;
	unsigned int line1 :BACTERIA_PER_FIELD_X;
	unsigned int left1 :1;
	
	unsigned int right2:1;
	unsigned int line2 :BACTERIA_PER_FIELD_X;
	unsigned int left2 :1;
	
	unsigned int right3:1;
	unsigned int line3 :BACTERIA_PER_FIELD_X;
	unsigned int left3 :1;
	
	unsigned int right4:1;
	unsigned int line4 :BACTERIA_PER_FIELD_X;
	unsigned int left4 :1;
	
	// 5 lines a 6 bits = 30 bits
	
	bool needsUpdateFromPast:1;
	bool wasChanged	   		:1;
};

typedef union field_u {
	uint32_t val;
	struct field_s bitfield;
} field_t;
#endif


#define BITS_IN_UINT32 (32)
#define FIELD_3X6LINE_LUT_NUM_ENTRIES (1 << (3 * (BACTERIA_PER_FIELD_X + 2/*left and right margin*/)))
#define FIELD_3X6LINE_LUT_SIZE ((FIELD_3X6LINE_LUT_NUM_ENTRIES) / (BITS_IN_UINT32 / BACTERIA_PER_FIELD_X))

void field_initLuts();

void field_update( field_t* field );


// from: http://stackoverflow.com/questions/109023/how-to-count-the-number-of-set-bits-in-a-32-bit-integer
static inline uint32_t myPopcount(uint32_t i)
{
	i = i - ((i >> 1) & 0x55555555);
	i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
	return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

#ifdef __OPENCL_VERSION__
#define FIELD_GLOBAL global
#define FIELD_LOCAL local
#else
#define FIELD_GLOBAL
#define FIELD_LOCAL
#endif

static inline void field_updateNeighbourCount( FIELD_GLOBAL field_t* field ) {


	field_t newField = *field;

	for ( int y = 0; y < BACTERIA_PER_FIELD_Y; y++ ) {
		for ( int x = 0; x < BACTERIA_PER_FIELD_X; x++ ) {
			
			uint32_t neighbours = (field->val >> (y*FIELD_LINE_WIDTH + x)) & FIELD_NEIGHBOUR_MASK;
			uint32_t numNeighbours = myPopcount(neighbours);
			
			bool bWasAlive = (field->val >> FIELD_ELEMENT_SHIFT_FOR_MASK(x+1,y+1)) & 1;
			
			bool bIsAlive = false;
			if( numNeighbours == 3 ) {
				bIsAlive = true;
			} else if ( bWasAlive && numNeighbours == 2 ) {
				bIsAlive = true;
			}
			
			newField.val &= ~(1 << FIELD_ELEMENT_SHIFT_FOR_MASK(x+1,y+1));
			newField.val |= bIsAlive << FIELD_ELEMENT_SHIFT_FOR_MASK(x+1,y+1);
			
			
// #ifdef __OPENCL_VERSION__
			newField.val |= (bIsAlive != bWasAlive) << FIELD_CHANGED_BIT_POS;
// #else
// 			newField.bitfield.wasChanged |= (bIsAlive != bWasAlive);
// #endif // __OPENCL_VERSION__
			
		}
	}
	*field = newField;
}



void static field_print( FIELD_GLOBAL field_t* field, unsigned int line)
{
#ifndef __OPENCL_VERSION__
	for ( int x = BACTERIA_PER_FIELD_X - 1; x >= 0; x-- ) {
		printf("%c", FIELD_GET_ELEMENT(*field, x, line) ? 'X' : '_');
	}
#endif
}

void  static field_printDebug( FIELD_GLOBAL field_t* field, int line)
{
#ifndef __OPENCL_VERSION__
	for ( int x = BACTERIA_PER_FIELD_X + 2 - 1; x >= 0; x-- ) {
		

		if( line == 0 || line == BACTERIA_PER_FIELD_Y + 1 || x == 0 || x == BACTERIA_PER_FIELD_X + 1) {
			printf("%c", ((field->val >> FIELD_ELEMENT_SHIFT_FOR_MASK( x, line)) & 1) ? 'o' : '_');
		} else {
			printf("%c", ((field->val >> FIELD_ELEMENT_SHIFT_FOR_MASK(x, line)) & 1) ? 'X' : ' ');
		}
		
	}
#endif
}

void static  field_printDebugAllLines(field_t* field)
{
#ifndef __OPENCL_VERSION__
	for( int i = BACTERIA_PER_FIELD_Y + 2 - 1; i >= 0; i-- ) {
		field_printDebug(field, i);
		printf("\n");
	}
#endif
}

// bool field_has_changed ( field_t* field );

#ifdef __OPENCL_VERSION__
void static inline field_broadcastLeft( FIELD_GLOBAL field_t* field, FIELD_GLOBAL field_t* neighbour )
{
	// clear right area of neighbour
	atomic_and ((global uint*)neighbour, UINT32_MAX & ~FIELD_ALL_NEIGHBOURS_RIGHT_MASK);
	// broadcast value
	uint32_t val = (field->val & FIELD_ALL_ELEMENTS_LEFT_MASK) >> BACTERIA_PER_FIELD_X;
	// set value
	atomic_or((global uint*)neighbour, val);	
}

void static inline field_broadcastTopLeft( FIELD_GLOBAL field_t* field, FIELD_GLOBAL field_t* neighbour )
{
	// clear right area of neighbour
	atomic_and ((global uint*)neighbour,  UINT32_MAX & ~FIELD_ALL_NEIGHBOURS_BOTTOM_RIGHT_MASK);
	// broadcast value
	uint32_t val = (field->val & FIELD_ALL_ELEMENTS_TOP_LEFT_MASK) >> (FIELD_ELEMENT_TL_POS - FIELD_NEIGHBOUR_BR_POS);
	// set value
	atomic_or((global uint*)neighbour, val);	
}


void static inline field_broadcastTop( FIELD_GLOBAL field_t* field, FIELD_GLOBAL field_t* neighbour )
{
	// clear right area of neighbour
	atomic_and ((global uint*)neighbour,  UINT32_MAX & ~FIELD_ALL_NEIGHBOURS_BOTTOM_MASK);
	// broadcast value
	uint32_t val = (field->val & FIELD_ALL_ELEMENTS_TOP_MASK) >> (BACTERIA_PER_FIELD_Y * FIELD_LINE_WIDTH);
	// set value
	atomic_or((global uint*)neighbour, val);	
}

void static inline field_broadcastTopRight( FIELD_GLOBAL field_t* field, FIELD_GLOBAL field_t* neighbour )
{
	// clear right area of neighbour
	atomic_and ((global uint*)neighbour,  UINT32_MAX & ~FIELD_ALL_NEIGHBOURS_BOTTOM_LEFT_MASK);
	// broadcast value
	uint32_t val = (field->val & FIELD_ALL_ELEMENTS_TOP_RIGHT_MASK) >> (FIELD_ELEMENT_TR_POS - FIELD_NEIGHBOUR_BL_POS);
	// set value
	atomic_or((global uint*)neighbour, val);	
}

void static inline field_broadcastRight( FIELD_GLOBAL field_t* field, FIELD_GLOBAL field_t* neighbour )
{
	// clear right area of neighbour
	atomic_and ((global uint*)neighbour,  UINT32_MAX & ~FIELD_ALL_NEIGHBOURS_LEFT_MASK);
	// broadcast value
	uint32_t val = (field->val & FIELD_ALL_ELEMENTS_RIGHT_MASK) << BACTERIA_PER_FIELD_X;
	// set value
	atomic_or((global uint*)neighbour, val);	
}

void static inline field_broadcastBottomRight(FIELD_GLOBAL field_t* field, FIELD_GLOBAL field_t* neighbour )
{
	// clear right area of neighbour
	atomic_and ((global uint*)neighbour,  UINT32_MAX & ~FIELD_ALL_NEIGHBOURS_TOP_LEFT_MASK);
	// broadcast value
	uint32_t val = (field->val & FIELD_ALL_ELEMENTS_BOTTOM_RIGHT_MASK) << (-FIELD_ELEMENT_BR_POS + FIELD_NEIGHBOUR_TL_POS);
	// set value
	atomic_or((global uint*)neighbour, val);	
}

void static inline field_broadcastBottom( FIELD_GLOBAL field_t* field, FIELD_GLOBAL field_t* neighbour )
{
	// clear right area of neighbour
	atomic_and ((global uint*)neighbour,  UINT32_MAX & ~FIELD_ALL_NEIGHBOURS_TOP_MASK);
	// broadcast value
	uint32_t val = (field->val & FIELD_ALL_ELEMENTS_BOTTOM_MASK) << (BACTERIA_PER_FIELD_Y * FIELD_LINE_WIDTH);

	// set value
	atomic_or((global uint*)neighbour, val);	
}

void static inline field_broadcastBottomLeft( FIELD_GLOBAL field_t* field, FIELD_GLOBAL field_t* neighbour )
{
	// clear right area of neighbour
	atomic_and ((global uint*)neighbour,  UINT32_MAX & ~FIELD_ALL_NEIGHBOURS_TOP_RIGHT_MASK);
	// broadcast value
	uint32_t val = (field->val & FIELD_ALL_ELEMENTS_BOTTOM_LEFT_MASK) << (-FIELD_ELEMENT_BL_POS + FIELD_NEIGHBOUR_TR_POS);
	// set value
	atomic_or((global uint*)neighbour, val);	
}

#else
void static inline field_broadcastLeft( FIELD_GLOBAL field_t* field, FIELD_GLOBAL field_t* neighbour )
{
	// clear right area of neighbour
	neighbour->val &= UINT32_MAX & ~FIELD_ALL_NEIGHBOURS_RIGHT_MASK;
	// broadcast value
	uint32_t val = (field->val & FIELD_ALL_ELEMENTS_LEFT_MASK) >> BACTERIA_PER_FIELD_X;
	// set value
	neighbour->val |= val;	
}

void static inline field_broadcastTopLeft( FIELD_GLOBAL field_t* field, FIELD_GLOBAL field_t* neighbour )
{
	// clear right area of neighbour
	neighbour->val &= UINT32_MAX & ~FIELD_ALL_NEIGHBOURS_BOTTOM_RIGHT_MASK;
	// broadcast value
	uint32_t val = (field->val & FIELD_ALL_ELEMENTS_TOP_LEFT_MASK) >> (FIELD_ELEMENT_TL_POS - FIELD_NEIGHBOUR_BR_POS);
	// set value
	neighbour->val |= val;	
}


void static inline field_broadcastTop( FIELD_GLOBAL field_t* field, FIELD_GLOBAL field_t* neighbour )
{
	// clear right area of neighbour
	neighbour->val &= UINT32_MAX & ~FIELD_ALL_NEIGHBOURS_BOTTOM_MASK;
	// broadcast value
	uint32_t val = (field->val & FIELD_ALL_ELEMENTS_TOP_MASK) >> (BACTERIA_PER_FIELD_Y * FIELD_LINE_WIDTH);
	// set value
	neighbour->val |= val;	
}

void static inline field_broadcastTopRight( FIELD_GLOBAL field_t* field, FIELD_GLOBAL field_t* neighbour )
{
	// clear right area of neighbour
	neighbour->val &= UINT32_MAX & ~FIELD_ALL_NEIGHBOURS_BOTTOM_LEFT_MASK;
	// broadcast value
	uint32_t val = (field->val & FIELD_ALL_ELEMENTS_TOP_RIGHT_MASK) >> (FIELD_ELEMENT_TR_POS - FIELD_NEIGHBOUR_BL_POS);
	// set value
	neighbour->val |= val;	
}

void static inline field_broadcastRight( FIELD_GLOBAL field_t* field, FIELD_GLOBAL field_t* neighbour )
{
	// clear right area of neighbour
	neighbour->val &= UINT32_MAX & ~FIELD_ALL_NEIGHBOURS_LEFT_MASK;
	// broadcast value
	uint32_t val = (field->val & FIELD_ALL_ELEMENTS_RIGHT_MASK) << BACTERIA_PER_FIELD_X;
	// set value
	neighbour->val |= val;	
}

void static inline field_broadcastBottomRight(FIELD_GLOBAL field_t* field, FIELD_GLOBAL field_t* neighbour )
{
	// clear right area of neighbour
	neighbour->val &= UINT32_MAX & ~FIELD_ALL_NEIGHBOURS_TOP_LEFT_MASK;
	// broadcast value
	uint32_t val = (field->val & FIELD_ALL_ELEMENTS_BOTTOM_RIGHT_MASK) << (-FIELD_ELEMENT_BR_POS + FIELD_NEIGHBOUR_TL_POS);
	// set value
	neighbour->val |= val;	
}

void static inline field_broadcastBottom( FIELD_GLOBAL field_t* field, FIELD_GLOBAL field_t* neighbour )
{
	// clear right area of neighbour
	neighbour->val &= UINT32_MAX & ~FIELD_ALL_NEIGHBOURS_TOP_MASK;
	// broadcast value
	uint32_t val = (field->val & FIELD_ALL_ELEMENTS_BOTTOM_MASK) << (BACTERIA_PER_FIELD_Y * FIELD_LINE_WIDTH);

	// set value
	neighbour->val |= val;	
}

void static inline field_broadcastBottomLeft( FIELD_GLOBAL field_t* field, FIELD_GLOBAL field_t* neighbour )
{
	// clear right area of neighbour
	neighbour->val &= UINT32_MAX & ~FIELD_ALL_NEIGHBOURS_TOP_RIGHT_MASK;
	// broadcast value
	uint32_t val = (field->val & FIELD_ALL_ELEMENTS_BOTTOM_LEFT_MASK) << (-FIELD_ELEMENT_BL_POS + FIELD_NEIGHBOUR_TR_POS);
	// set value
	neighbour->val |= val;	
}
#endif


#endif // FIELD_H