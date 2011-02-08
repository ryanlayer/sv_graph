#ifndef __SFX_EDGE_H__
#define __SFX_EDGE_H__

class sfx_edge {
public:
	sfx_edge(int first, int last, int parent, int count);
	sfx_edge();
	int first_char_index, last_char_index, start_node,
		end_node;
};

#endif
