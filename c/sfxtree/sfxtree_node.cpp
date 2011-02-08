#include <map>

using namespace std;

#include "sfxtree.h"

sfxtree_node::sfxtree_node(int o, int l, int s) {
	offset = o;
	length = l;
	starts_at = s;
	sfx_link = NULL;
}

sfxtree_node::sfxtree_node() {
	offset = 0;
	length = 0;
	starts_at = 0;
	sfx_link = NULL;
}

void sfxtree_node::set_offset(int o) {
	offset = o;
}

void sfxtree_node::set_length(int l) {
	length = l;
}

void sfxtree_node::set_sfx_link(sfxtree_node *l) {
	sfx_link = l;
}

sfxtree_node* sfxtree_node::get_sfx_link() {
	return sfx_link;
}

int sfxtree_node::get_offset() {
	return offset;
}

int sfxtree_node::get_length() {
	return length;
}

int sfxtree_node::get_starts_at() {
	return starts_at;
}

map<char, sfxtree_node*> sfxtree_node::get_children() {
	return children;
}

sfxtree_node* sfxtree_node::get_child(char c) {
	map<char,sfxtree_node*>::iterator it;
	it = children.find(c);

	if (it == children.end())
		return NULL;
	else
		return children[c];
}

void sfxtree_node::add_child(char c, int o, int l, int s) {
	children[c] = new sfxtree_node(o, l, s);
}

void sfxtree_node::add_child(char c, sfxtree_node *p) {
	children[c] = p;
}


void sfxtree_node::set_child(char c, int o, int l, int s) {
	add_child(c, o, l, s);	
}
