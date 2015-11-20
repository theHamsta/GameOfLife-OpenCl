
#define BOARD_LINE_SKIP (BOARD_WIDTH + 2 * BOARD_PADDING_X)
#define BOARD_GET_FIELD_IDX(X,Y)  (((X) + BOARD_PADDING_X) + ((Y) + BOARD_PADDING_Y) * BOARD_LINE_SKIP )


kernel void broadcastNeighbourhoodsDevice (
	global field_t* buffer
)
{
	uint globalIdx = get_global_id(0);
	uint localIdx = get_local_id(0);
	

	if ( globalIdx >= BOARD_WIDTH ) {
		return;
	}
	
	for ( uint y = 0; y < BOARD_HEIGHT; y++ ) {
		
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
