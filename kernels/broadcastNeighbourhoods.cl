
#define BOARD_LINE_SKIP (BOARD_WIDTH + 2 * BOARD_PADDING_X)
#define BOARD_GET_FIELD_IDX(X,Y)  (((X) + BOARD_PADDING_X) + ((Y) + BOARD_PADDING_Y) * BOARD_LINE_SKIP )


kernel void broadcastNeighbourhoodsDevice (
	global field_t* buffer
)
{
	uint globalIdx = get_global_id(0);
	uint localIdx = get_local_id(0);
	
	
// 	for ( uint y = 0; y < BOARD_HEIGHT; y++ ) {
// 		if ( globalIdx < BOARD_WIDTH ) {
// 			global field_t* current = buffer + BOARD_GET_FIELD_IDX( globalIdx, y );
// 			current->val = UINT32_MAX & FIELD_ALL_ELEMENTS_MASK;
// 		}
// 	}
// 	barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);
	

	local field_t sharedMem[ (LOCAL_SIZE + 2) * 3 ];
	
	local field_t* above = sharedMem;
	local field_t* cur = above + (LOCAL_SIZE + 2);
	local field_t* below = cur + (LOCAL_SIZE + 2);

	
	if (localIdx == 0 ){
		above[0].val = 0;
		above[ LOCAL_SIZE + 1 ].val = 0;
		cur[0].val = 0;
		cur[ LOCAL_SIZE + 1 ].val = 0;
		below[0].val = 0;
		below[ LOCAL_SIZE + 1 ].val = 0;
	}
	if ( globalIdx >= BOARD_WIDTH ) {
		return;
	}
	
	for ( uint y = 0; y < BOARD_HEIGHT; y++ ) {
		
// 		above	[ 1 + localId ] = 0 ;
// 		
// 		below	[ 1 + localId ] = 0 ;
	
		
		global field_t* current = buffer + BOARD_GET_FIELD_IDX( globalIdx, y ) ;
		


		if ( FIELD_WAS_CHANGED(*current) ) {
			
			field_broadcastLeft			(current, buffer + BOARD_GET_FIELD_IDX(globalIdx - 1, y + 0));
			field_broadcastTopLeft		(current, buffer + BOARD_GET_FIELD_IDX(globalIdx - 1, y - 1));
			field_broadcastTop			(current, buffer + BOARD_GET_FIELD_IDX(globalIdx + 0, y - 1));
			field_broadcastTopRight		(current, buffer + BOARD_GET_FIELD_IDX(globalIdx + 1, y - 1));
			field_broadcastRight		(current, buffer + BOARD_GET_FIELD_IDX(globalIdx + 1, y + 0));
			field_broadcastBottomRight	(current, buffer + BOARD_GET_FIELD_IDX(globalIdx + 1, y + 1));
			field_broadcastBottom		(current, buffer + BOARD_GET_FIELD_IDX(globalIdx + 0, y + 1));
			field_broadcastBottomLeft	(current, buffer + BOARD_GET_FIELD_IDX(globalIdx - 1, y + 1));
			
			FIELD_UNSET_WAS_CHANGED(*current);
		}
	}
}
	