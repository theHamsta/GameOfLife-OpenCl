#define BOARD_LINE_SKIP (BOARD_WIDTH + 2 * BOARD_PADDING_X)
#define BOARD_GET_FIELD_IDX(X,Y)  (((X) + BOARD_PADDING_X) + ((Y) + BOARD_PADDING_Y) * BOARD_LINE_SKIP )

#define LOCAL_SIZE (get_local_size(0))
#define SHARED_MEM_SIZE  (3 * ( get_local_size(0) + 2))


// Fix overlapping regions by xor-ing overlappingRegions buffer with corresponding region in "buffer"

kernel void fixOverlappingRegions (
	global field_t* buffer,
	global field_t* overlappingRegions // between global work groups
)
{
	
	uint globalId = get_global_id(0);
	uint localId = get_local_id(0);
	uint localWorkGroupId = globalId / LOCAL_SIZE;
	uint numGlobalLocalWorkGroups = get_global_size(0) / LOCAL_SIZE;
	

	
	uint blockHeight = (BOARD_HEIGHT - 1) / numGlobalLocalWorkGroups + 1;
	
	uint y_end = (localWorkGroupId + 1) * blockHeight;
	if ( y_end > BOARD_HEIGHT ) {
		y_end = BOARD_HEIGHT;
	}
	
	for ( uint x = localId; x < ((BOARD_WIDTH - 1) / LOCAL_SIZE + 1) * LOCAL_SIZE /* ensure that warps do not diverge or else sync is impossible!!!*/; x += LOCAL_SIZE ) {
		if ( x < BOARD_WIDTH ) {
			buffer[ BOARD_GET_FIELD_IDX(x, y_end - 1) ].val ^= overlappingRegions[ localWorkGroupId * BOARD_LINE_SKIP + x + 1 ].val;
		}
	}
	
}
	