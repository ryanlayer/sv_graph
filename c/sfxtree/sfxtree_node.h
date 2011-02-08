#ifndef __SFXTREE_NODE_H__
#define __SFXTREE_NODE_H__

#include <map>

using namespace std;

class sfxtree_node {
	public:
		sfxtree_node();
		sfxtree_node(int o, int l, int s);
		void set_offset(int o);
		void set_length(int o);
		void set_sfx_link(sfxtree_node *l);
		sfxtree_node* get_sfx_link();
		int get_offset();
		int get_length();
		int get_starts_at();
		sfxtree_node* get_child(char c);
		map<char,sfxtree_node*> get_children();
		void add_child(char c, int o, int l, int s);
		void add_child(char c, sfxtree_node *p);
		void set_child(char c, int o, int l, int s);
	private:
		int offset;
		int length;
		int starts_at;
		map<char, sfxtree_node*> children;
		sfxtree_node* sfx_link;
};

#endif
