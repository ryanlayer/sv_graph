#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

#include "sv_graph_node.h"
#include "sv_graph_component.h"

int sv_graph_node::NM = 1;
int sv_graph_node::U = 0;
int sv_graph_node::white = 0;
int sv_graph_node::black = 1;
int sv_graph_node::gray = 2;
int sv_graph_node::green = 3;

sv_graph_node::
sv_graph_node(vector<string> *val) {

	/*
	 * N,ATGACCTTTTGACTTTACTGCCTACCTCCCAACTCCCG,
	 * U,10091349,>gi|51511750|ref|NC_000021.7|NC_000021,60,82
	 *
	 * 0) type
	 * 1) sequence or offset
	 * 2) chr
	 * 3) score
	 * 4) quality
	 */

	if ( (*val)[0].compare("N") == 0 ) {
		offset = -1;
		score = -1;
		quality = -1;
		type = NM;
		seq = (*val)[1];
	} else if ( (*val)[0].compare("U") == 0 ) {
		chr = (*val)[2];
		offset = atoi( (*val)[1].c_str() );
		type = U;
		score = atoi( (*val)[3].c_str() );
		quality = atoi( (*val)[4].c_str() );
	} 

	in = new vector<sv_edge*>;
	out = new vector<sv_edge*>;
	color = white;
	pair = NULL;
}

sv_graph_node::
sv_graph_node(string val) {
	if ( isdigit(val[0]) ){ // is an offset
		offset = atoi( val.c_str() );
		type = U;
	} else { // in a sequence
		offset = -1;
		type = NM;
		seq = val;
	}
	score = -1;
	quality = -1;
	in = new vector<sv_edge*>;
	out = new vector<sv_edge*>;
	color = white;
	pair = NULL;
}

string
sv_graph_node::
get_chr()
{
	return chr;
}

int
sv_graph_node::
get_color()
{
	return color;
}

void
sv_graph_node::
set_color(int c)
{ 
	color = c; 
}

int
sv_graph_node::
get_type()
{
	return type;
}

int
sv_graph_node::
get_offset()
{
	return offset;
}

void
sv_graph_node::
join_pair(sv_graph_node *p)
{
	pair = p;
}

sv_graph_node*
sv_graph_node::
get_pair()
{
	return pair;
}

string
sv_graph_node::
get_seq()
{
	return seq;
}

int
sv_graph_node::
get_score()
{
	return score;
}

int
sv_graph_node::
get_quality()
{
	return score;
}

vector<sv_edge*>*
sv_graph_node::
get_out_edges()
{
	return out;
}

vector<sv_edge*>*
sv_graph_node::
get_in_edges()
{
	return in;
}

void
sv_graph_node::
add_in_edge(sv_edge *edge)
{
	in->push_back(edge);	
}

void
sv_graph_node::
add_edge(sv_graph_node *p, int w)
{
	sv_edge *edge = new sv_edge(this, p, w, 0); 
	out->push_back(edge);	
	p->add_in_edge(edge);
}

void
sv_graph_node::
print_out_edges()
{
	vector<sv_edge*>::const_iterator i;
	cout << "[ " ;
	for (i = out->begin(); i != out->end(); i++) {
		cout << (*i)->get_weight() << ":";
		sv_graph_node *n = (*i)->get_target();
		if (n->get_type() == sv_graph_node::NM)
			cout << n->get_seq() << " ";
		else 
			cout << n->get_offset() << " ";
	}
	cout << "]" << endl;
}

void
sv_graph_node::
get_connected_nodes(sv_graph_component *c)
{
	if (color == green)
		return;

	c->add(this);
	color = green;

	vector<sv_edge*>::const_iterator i;
	sv_graph_node *u;
	for (i = out->begin(); i != out->end(); i++) {
		u = (*i)->get_target();
		u->get_connected_nodes(c);
	}
	for (i = in->begin(); i != in->end(); i++) {
		u = (*i)->get_source();
		u->get_connected_nodes(c);
	}
}

bool
sv_graph_node::
compare_sv_graph_nodes(sv_graph_node *first, sv_graph_node *second)
{
	if (	(first->get_type() == NM) ||
			(second->get_type()	== NM) )
		return false;

	return first->get_offset() < second->get_offset();
}

bool
sv_graph_node::
//operator<(const sv_edge &a, const sv_edge &b) const
operator<(sv_graph_node &other)
{
	sv_graph_node *a, *b;
	
	if (type == NM)
		a = pair;
	else
		a = this;
	
	if (other.get_type() == NM)
		b = other.get_pair();
	else
		b = &other;

	return a->get_offset() < b->get_offset();
}
