#ifndef __SFX_TREE_H__
#define __SFX_TREE_H__
#include <string>
using namespace std;

#include "sfx.h"
#include "sfx_edge.h"
#include "sfx_node.h"

class sfx_tree {

public:
	static int MAX_LENGTH, HASH_TABLE_SIZE;
	static char END_CHAR;
	~sfx_tree();
	sfx_tree(string t);
	void add_prefix(sfx &active, int last_char_index);
	sfx_edge find_edge(int node, int c);
	int hash(int node, int c);
	int split_edge(sfx &s, sfx_edge &e);
	void remove_edge(sfx_edge &e);
	void insert_edge(sfx_edge &e);
	void canonize(sfx &s);
	void search(char *query, int &start_index, int &end_index);
	int overlap(char *query);
	int overlap(string query);

private:
	char *T;
	int N;
	sfx_edge *edges;
	int count;
	sfx_node *nodes;
};

#endif
