
#define BOARD_LINE_SKIP (BOARD_WIDTH + 2 * BOARD_PADDING_X)
#define BOARD_GET_FIELD_IDX(X,Y)  (((X) + BOARD_PADDING_X) + ((Y) + BOARD_PADDING_Y) * BOARD_LINE_SKIP )


kernel void clearBufferDevice (
	global field_t* buffer
)
{
	uint globalIdx = get_global_id(0);
	

	if ( globalIdx >= BOARD_WIDTH ) {
		return;
	}
	
	for ( uint y = 0; y < BOARD_HEIGHT; y++ ) {
		
		buffer[BOARD_GET_FIELD_IDX( globalIdx, y )].val = 0;
		
	}
}