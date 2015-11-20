
#define BOARD_LINE_SKIP (BOARD_WIDTH + 2 * BOARD_PADDING_X)
#define BOARD_GET_FIELD_IDX(X,Y)  (((X) + BOARD_PADDING_X) + ((Y) + BOARD_PADDING_Y) * BOARD_LINE_SKIP )


kernel void broadcastNeighbourhoodsDevice(
	global field_t* buffer
)
{
	uint idx = get_global_id(0);

	
	if ( idx >= BOARD_WIDTH ) {
		return;
	}

	
	for ( uint y = 0; y < BOARD_HEIGHT; y++ ) {
		
		
		global field_t* current = buffer + BOARD_GET_FIELD_IDX(idx, y) ;
		
// 		buffer[BOARD_GET_FIELD_IDX(idx, y)].val = 37737;
		if ( FIELD_WAS_CHANGED(current) ) {
			field_broadcastLeft			(current, buffer + BOARD_GET_FIELD_IDX(idx - 1, y + 0));
			field_broadcastTopLeft		(current, buffer + BOARD_GET_FIELD_IDX(idx - 1, y - 1));
			field_broadcastTop			(current, buffer + BOARD_GET_FIELD_IDX(idx + 0, y - 1));
			field_broadcastTopRight		(current, buffer + BOARD_GET_FIELD_IDX(idx + 1, y - 1));
			field_broadcastRight		(current, buffer + BOARD_GET_FIELD_IDX(idx + 1, y + 0));
			field_broadcastBottomRight	(current, buffer + BOARD_GET_FIELD_IDX(idx + 1, y + 1));
			field_broadcastBottom		(current, buffer + BOARD_GET_FIELD_IDX(idx + 0, y + 1));
			field_broadcastBottomLeft	(current, buffer + BOARD_GET_FIELD_IDX(idx - 1, y + 1));
			
			FIELD_UNSET_WAS_CHANGED(*current);
		}
	}
}
	