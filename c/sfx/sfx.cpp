#include "sfx.h"
#include "sfx_edge.h"

sfx::sfx(int node, int start, int stop)
{
	origin_node = node;
	first_char_index = start;
	last_char_index = stop;
}

int 
sfx::
expl()
{
	return first_char_index > last_char_index;
}

int
sfx::
impl()
{
	return last_char_index >= first_char_index;
}
