#define BOARD_LINE_SKIP (BOARD_WIDTH + 2 * BOARD_PADDING_X)
#define BOARD_GET_FIELD_IDX(X,Y)  (((X) + BOARD_PADDING_X) + ((Y) + BOARD_PADDING_Y) * BOARD_LINE_SKIP )

#define LOCAL_SIZE (get_local_size(0))
#define SHARED_MEM_SIZE  (3 * ( get_local_size(0) + 2))

void inline memClear( local field_t* mem, uint memElements ){
	barrier(CLK_LOCAL_MEM_FENCE);
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
	global field_t* restrict buffer,
	global field_t* restrict upperOverlappingRegions, // between local work groups
	global field_t* restrict lowerOverlappingRegions, // between local work groups
	local field_t* sharedMem,
	local field_t* restrict verticalOverlappingRegions
)
{
	uint globalId = get_global_id(0);
	uint localId = get_local_id(0);
	uint localWorkGroupId = globalId / LOCAL_SIZE;
	uint numGlobalLocalWorkGroups = get_global_size(0) / LOCAL_SIZE;
	
	
	local field_t* abv = sharedMem;
	local field_t* cur = abv + (LOCAL_SIZE + 2);
	local field_t* blw = cur + (LOCAL_SIZE + 2);
	

			
	
	uint blockHeight = (BOARD_HEIGHT - 1) / numGlobalLocalWorkGroups + 1;
	uint y_start = localWorkGroupId * blockHeight;
	uint y_end = (localWorkGroupId + 1) * blockHeight;
	if ( y_end > BOARD_HEIGHT ) {
		y_end = BOARD_HEIGHT;
	}
	

	
	for ( uint x = localId; x < ((BOARD_WIDTH - 1) / LOCAL_SIZE + 1) * LOCAL_SIZE /* ensure that warps do not diverge or else sync is impossible!!!*/; x += LOCAL_SIZE ) {
		
		memClear( sharedMem, SHARED_MEM_SIZE );
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
			if (x < BOARD_WIDTH ) {
	 					//			if( x == 1 ) {
	 					//field_t go = abv[ 2 + localId ];
	 					//field_printDebugAllLines(&go);
	 					//go = cur[ 2 + localId ];
	 					//field_printDebugAllLines(&go);
	 					//go = blw[ 2 + localId ];
	 					//field_printDebugAllLines(&go);
	 					//printf("y=%i\n", y);
	 				//}
				if ( localId == 0 && y != y_start && x != 0) {
					abv[ 1 ].val |= verticalOverlappingRegions[ y - 1 - y_start ].val;
				}
				
				if ( y != y_start ) {
					if (abv[ 1 + localId ].val) {
						atomic_xor( (global uint*) buffer + BOARD_GET_FIELD_IDX( x, y - 1 ) , abv[ 1 + localId ].val);
					}
				} else if ( y != 0 ) {
					upperOverlappingRegions[ localWorkGroupId * BOARD_WIDTH + x ].val = abv[ 1 + localId ].val;
				}
				
				
				if ( localId == 0 &&  abv[ 0 ].val) {
					if ( y != y_start ) {
						atomic_xor( (global uint*) buffer + BOARD_GET_FIELD_IDX( x - 1, y - 1 ) , abv[ 0 ].val);
					} else {
						if (x!=0) {
							upperOverlappingRegions[ localWorkGroupId * BOARD_WIDTH + x - 1].val |= abv[ 0 ].val; 
						}
					}
				}
				if ( localId == 0 && x < BOARD_WIDTH && y != y_start ) {
					verticalOverlappingRegions[ y - 1 - y_start ].val = abv[ LOCAL_SIZE + 1 ].val;
				}

				
			}
			barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);
		}

		if ( x < BOARD_WIDTH ) {
			
			if ( localId == 0 && x < BOARD_WIDTH && x != 0 ) {
				cur[ 1 ].val |= verticalOverlappingRegions[ y_end - y_start - 1  ].val;
				blw[ 1 ].val |= verticalOverlappingRegions[ y_end - y_start ].val;
			}
			
			if ( localId == 0 && x < BOARD_WIDTH ) {
				verticalOverlappingRegions[ y_end - y_start - 1 ].val = cur[ LOCAL_SIZE + 1 ].val;
				verticalOverlappingRegions[ y_end - y_start ].val = blw[ LOCAL_SIZE + 1 ].val;
			}
		}
		barrier(CLK_LOCAL_MEM_FENCE );
		// Last row is an overlapping region with the next work group.
		// Save this row to overlappingRegions buffer to avoid synchronizing with other work group
		if ( x < BOARD_WIDTH ) {
			
			atomic_xor( (global uint*) buffer + BOARD_GET_FIELD_IDX( x, y_end - 1 ) , cur[ 1 + localId ].val);
			if ( y_end != BOARD_HEIGHT ) {
				lowerOverlappingRegions[ localWorkGroupId * BOARD_WIDTH + x ].val = blw[ 1 + localId ].val;
			}
			if (localId == 0 && x != 0) {
				atomic_xor( (global uint*) buffer + BOARD_GET_FIELD_IDX( x - 1, y_end - 1 ) , cur[ 0 ].val);
				if ( y_end != BOARD_HEIGHT ) {
					lowerOverlappingRegions[ localWorkGroupId * BOARD_WIDTH + x - 1].val |= blw[ 0 ].val;
				}
			}
			
		}
		barrier(CLK_LOCAL_MEM_FENCE);
	}
	


}
	