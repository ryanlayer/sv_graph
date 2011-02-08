#include <iostream> 
#include <fstream>
#include <string>
#include <cstdio>
#include <string.h>
#include <limits.h>
#include <vector>
#include <algorithm>

using namespace std;

#include "sfxtree/sfxtree.h"
#include "sv_graph/sv_graph_node.h"
#include "sv_graph/sv_graph_component.h"

//{{{ void sfx_connect_nodes( vector<sv_graph_node*> *nodes, int threshold ) {
void sfx_connect_nodes( vector<sv_graph_node*> *nodes, int threshold ) {

	// N^2 pair matching
	vector<sv_graph_node*>::const_iterator x_iter, y_iter;

	for (x_iter = nodes->begin(); x_iter != nodes->end(); x_iter++) {
		sv_graph_node *xp = *x_iter;
		sfxtree x_tree( xp->get_seq() );

		for (y_iter = nodes->begin(); y_iter != nodes->end(); y_iter++) {
			if (x_iter != y_iter) {
				sv_graph_node *yp = *y_iter;
				int max_sfx = x_tree.max_suffix( yp->get_seq() );
				if ( max_sfx >= threshold ) {
					xp->add_edge(yp, max_sfx);
				}
			}
		}
	}
}
//}}}

void search2(sv_graph_node *v, string *contig, int *w) {

	v->set_color(sv_graph_node::black);

	vector<sv_edge*> *edges = v->get_out_edges();
	vector<sv_edge*>::const_iterator i;

	int max_child_weight = -1;
	sv_graph_node *t = NULL, *u = NULL;

	for (i = edges->begin(); i != edges->end(); i++) {
		int u_w = (*i)->get_weight();
		u = (*i)->get_target();

		if (    ( u->get_color() != sv_graph_node::black ) &&
				( u_w > max_child_weight ) ) {
			max_child_weight = u_w;
			t = u;
		}
	}

	if ( max_child_weight > -1 ) {
		string s = t->get_seq();
		*w = *w + max_child_weight;
		*contig += s.substr(max_child_weight, s.length() - max_child_weight);
		search2(t, contig, w);
	}

	v->set_color(sv_graph_node::white);
}

int main(int argc, char** anrgv) {

	vector<string> seqs;
	vector<sv_graph_node*> nodes;

	char s[LINE_MAX];
	while ( fgets(s, LINE_MAX, stdin) ) {
		string t(s, strlen(s) - 1);
		seqs.push_back(t);
		sv_graph_node *n = new sv_graph_node(t);
		nodes.push_back(n);
	}

	sfx_connect_nodes(&nodes, 20);

	vector< sv_graph_component* > *components = 
			new vector< sv_graph_component*>();


	vector<sv_graph_node*>::const_iterator j;
	for (j = nodes.begin(); j != nodes.end(); j++) {
		//find connected components
		sv_graph_component *component = 
				new sv_graph_component();
		sv_graph_node *v = (*j);
		v->get_connected_nodes(component);
		if (component->size() >= 1) 
			components->push_back(component);
	}


	vector< sv_graph_component* >::const_iterator k;
	for (k = components->begin(); k != components->end(); k++) {
		cout << "component size: " << (*k)->size() << endl;
		vector<sv_graph_node*>::const_iterator l;

		int max = -1;
		string s = "";
		for (l = (*k)->get_nodes()->begin(); l != (*k)->get_nodes()->end();
				l++) {
			int bw = 0;
			sv_graph_node *v = *l;
			string contig = v->get_seq();
			search2(v, &contig, &bw);

			if (bw > max) {
				max = bw;
				s = contig;
			}
		}
		cout << s << "\t" << max << endl;
	}

	return 0;
}
