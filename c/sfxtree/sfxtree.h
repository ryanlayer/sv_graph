#ifndef __SFXTREE_H__
#define __SFXTREE_H__

#include <string>

using namespace std;

#include "sfxtree_node.h"

class sfxtree {
	public:
		sfxtree(string t);
		void add(int *i, int *j, sfxtree_node *p, int *p_depth, 
				sfxtree_node *need_sfx);
		int max_suffix(string query);
		int max_suffix_2(string query);
		void print_tree();
	private:
		string target;
		sfxtree_node root;
};

#endif
