#ifndef __SFX_H__
#define __SFX_H__

class sfx{
public:
	sfx(int node, int start, int stop);
	int expl();
	int impl();
	int origin_node, first_char_index, last_char_index;
};

#endif
