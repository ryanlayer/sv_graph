#include "sfx_edge.h"

sfx_edge::
sfx_edge() {
	start_node = -1;	
}

sfx_edge::
sfx_edge(int first, int last, int parent, int count){
	first_char_index = first;
	last_char_index = last;
	start_node = parent;
	end_node = count;
}
