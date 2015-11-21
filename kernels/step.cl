#define BOARD_LINE_SKIP (BOARD_WIDTH + 2 * BOARD_PADDING_X)
#define BOARD_GET_FIELD_IDX(X,Y)  (((X) + BOARD_PADDING_X) + ((Y) + BOARD_PADDING_Y) * BOARD_LINE_SKIP )

#define LOCAL_SIZE (get_local_size(0))
#define SHARED_MEM_SIZE  (3 * ( get_local_size(0) + 2))

void inline memClear( local field_t* mem, uint memElements ){
	for ( uint idx = get_local_id(0); idx < memElements; idx += LOCAL_SIZE ) {
		mem[idx].val = 0;
	}
	barrier(CLK_LOCAL_MEM_FENCE);
}

void inline rotateSharedMemPointers( local field_t** a, local field_t** b, local field_t** c ) {
	local field_t* tmp = *a;
	*a = *b;
	*b = *c;
	*c = tmp;
}


kernel void stepDevice (
	global field_t* buffer,
	global field_t* overlappingRegions, // between global work groups
	local field_t* sharedMem
)
{
	
	
	uint globalId = get_global_id(0);
	uint localId = get_local_id(0);
	uint localWorkGroupId = globalId / LOCAL_SIZE;
	uint numGlobalLocalWorkGroups = get_global_size(0) / LOCAL_SIZE;
	
// 	local field_t sharedMem[ 3 * ( LOCAL_SIZE + 2) ];
	
	local field_t* abv = sharedMem;
	local field_t* cur = abv + (LOCAL_SIZE + 2);
	local field_t* blw = cur + (LOCAL_SIZE + 2);
	

	memClear( sharedMem, SHARED_MEM_SIZE );
	
	uint blockHeight = (BOARD_HEIGHT - 1) / numGlobalLocalWorkGroups + 1;
	uint y_start = localWorkGroupId * blockHeight;
	uint y_end = (localWorkGroupId + 1) * blockHeight;
	if ( y_end > BOARD_HEIGHT ) {
		y_end = BOARD_HEIGHT;
	}
	
	for ( uint x = localId; x < ((BOARD_WIDTH - 1) / LOCAL_SIZE + 1) * LOCAL_SIZE /* ensure that warps do not diverge or else sync is impossible!!!*/; x += LOCAL_SIZE ) {
		for ( uint y = y_start; y < y_end; y++ ) {

			rotateSharedMemPointers( &abv, &cur, &blw );
			
			uint fieldDiff;
			
			memClear( blw, LOCAL_SIZE + 2 );
			
			// calculate the bits that need to be changed in the current field
			if ( x < BOARD_WIDTH ) {
				field_t oldField = buffer[BOARD_GET_FIELD_IDX( x, y )];
				field_t newField = field_getUpdatedFieldNeighbourCount( oldField );
				fieldDiff = (oldField.val ^ newField.val);
				atomic_or(&cur[ 1 + localId ].val, fieldDiff);
				
				if( localId == 0 ) {
					blw[ 0 ].val = 0;
					blw[ 1 + LOCAL_SIZE ].val = 0;
				}
			}

			// if something changed, propagate these changes
			if ( fieldDiff && x < BOARD_WIDTH ) {
				
				field_broadcastDiffLeft			(fieldDiff, &cur[ 1 + localId - 1 ]);
				field_broadcastDiffTopLeft		(fieldDiff, &abv[ 1 + localId - 1 ]);
				field_broadcastDiffTop			(fieldDiff, &abv[ 1 + localId + 0 ]);
				field_broadcastDiffTopRight		(fieldDiff, &abv[ 1 + localId + 1 ]);
				field_broadcastDiffRight		(fieldDiff, &cur[ 1 + localId + 1 ]);
				field_broadcastDiffBottomRight	(fieldDiff, &blw[ 1 + localId + 1 ]);
				field_broadcastDiffBottom		(fieldDiff, &blw[ 1 + localId + 0 ]);
				field_broadcastDiffBottomLeft	(fieldDiff, &blw[ 1 + localId - 1 ]);
				
			}
			

			// sync and save changes to global memory
			barrier(CLK_LOCAL_MEM_FENCE);
			if ( y != 0 && x < BOARD_WIDTH && abv[ 1 + localId ].val ) {
				atomic_xor( (global uint*) buffer + BOARD_GET_FIELD_IDX( x, y - 1 ) , abv[ 1 + localId ].val);
			}
			
			if ( localId == 0 && abv[ 0 ].val) {
				atomic_xor( (global uint*) buffer + BOARD_GET_FIELD_IDX( x - 1, y - 1 ) , abv[ 0 ].val);
			}
			if ( localId == LOCAL_SIZE - 1 && x < BOARD_WIDTH && abv[ LOCAL_SIZE + 1 ].val) {
				atomic_xor( (global uint*) buffer + BOARD_GET_FIELD_IDX( x + 1, y - 1 ) , abv[ LOCAL_SIZE + 1 ].val);
			}
		}

	
		// Last row is an overlapping region with the next work group.
		// Save this row  to overlappingRegions buffer to avoid synchronizing with other work group
		barrier(CLK_LOCAL_MEM_FENCE);
		if ( x < BOARD_WIDTH ) {
			overlappingRegions[ localWorkGroupId * BOARD_LINE_SKIP + x + 1 ].val = cur[ 1 + localId ].val;
		}
		
		if ( localId == 0 &&  cur[ 0 ].val ) {
			overlappingRegions[ localWorkGroupId * BOARD_LINE_SKIP + x + 1 - 1 ].val |= cur[ 0 ].val;
		}
		if ( localId == LOCAL_SIZE - 1 && x < BOARD_WIDTH && cur[ LOCAL_SIZE + 1 ].val) {
			overlappingRegions[ localWorkGroupId * BOARD_LINE_SKIP + x + 1 + 1 ].val |= cur[ LOCAL_SIZE + 1 ].val;
		}
	}
	
}
	