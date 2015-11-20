#define BOARD_LINE_SKIP (BOARD_WIDTH + 2 * BOARD_PADDING_X)
#define BOARD_GET_FIELD_IDX(X,Y)  (((X) + BOARD_PADDING_X) + ((Y) + BOARD_PADDING_Y) * BOARD_LINE_SKIP )

#define LOCAL_SIZE (get_local_size(0))
#define SHARED_MEM_SIZE  (3 * ( get_local_size(0) + 2))

void inline clearSharedMem( local field_t* sharedMem ){
	for ( uint idx = get_local_id(0); idx < SHARED_MEM_SIZE; idx += LOCAL_SIZE ) {
		sharedMem[idx].val = 0;
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
	local field_t* sharedMem
)
{
	
	
	uint globalId = get_global_id(0);
	uint localId = get_local_id(0);
	
// 	local field_t sharedMem[ 3 * ( LOCAL_SIZE + 2) ];
	
	local field_t* abv = sharedMem;
	local field_t* cur = abv + (LOCAL_SIZE + 2);
	local field_t* blw = cur + (LOCAL_SIZE + 2);
	

	clearSharedMem( sharedMem );
	

	
	for ( uint y = 0; y < BOARD_HEIGHT; y++ ) {

		rotateSharedMemPointers( &abv, &cur, &blw );
		
		uint fieldDiff;
		
		// calculate the bits that need to be changed in the current field
		if ( globalId < BOARD_WIDTH ) {
			field_t oldField = buffer[BOARD_GET_FIELD_IDX( globalId, y )];
			field_t newField = field_getUpdatedFieldNeighbourCount( oldField );
			fieldDiff = (oldField.val ^ newField.val);
			cur[ 1 + localId ].val = fieldDiff;
			
			if( localId == 0 ) {
				blw[ 0 ].val = 0;
				blw[ 1 + LOCAL_SIZE ].val = 0;
			}
		}

		// if something changed, propagate these changes
		if ( fieldDiff && globalId < BOARD_WIDTH ) {
			
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
		if ( y != 0 && globalId < BOARD_WIDTH && abv[ 1 + localId ].val ) {
			atomic_xor( (global uint*) buffer + BOARD_GET_FIELD_IDX( globalId, y - 1 ) , abv[ 1 + localId ].val);
		}
		
		if ( localId == 0 ) {
			atomic_xor( (global uint*) buffer + BOARD_GET_FIELD_IDX( globalId - 1, y - 1 ) , abv[ 0 ].val);
		}
		if ( localId == LOCAL_SIZE - 1 && globalId < BOARD_WIDTH ) {
			atomic_xor( (global uint*) buffer + BOARD_GET_FIELD_IDX( globalId + 1, y - 1 ) , abv[ LOCAL_SIZE + 1 ].val);
		}
	}
	
	// sync and save changes to global memory
	barrier(CLK_LOCAL_MEM_FENCE);
	if ( globalId < BOARD_WIDTH && cur[ 1 + localId ].val ) {
		atomic_xor( (global uint*) buffer + BOARD_GET_FIELD_IDX( globalId, BOARD_HEIGHT - 1 ) , cur[ 1 + localId ].val);
	}
	
	if ( localId == 0 ) {
		atomic_xor( (global uint*) buffer + BOARD_GET_FIELD_IDX( globalId - 1, BOARD_HEIGHT - 1 ) , cur[ 0 ].val);
	}
	if ( localId == LOCAL_SIZE - 1 && globalId < BOARD_WIDTH ) {
		atomic_xor( (global uint*) buffer + BOARD_GET_FIELD_IDX( globalId + 1, BOARD_HEIGHT - 1) , cur[ LOCAL_SIZE + 1 ].val);
	}
	
	
}
	