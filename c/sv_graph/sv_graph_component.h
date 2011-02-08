class sv_graph_component;
#ifndef __SV_GRAPH_COMPONENT__
#define __SV_GRAPH_COMPONENT__

#include <vector>

using namespace std;

#include "sv_graph_node.h" 

class sv_graph_component 
{

public:
	sv_graph_component();
	sv_graph_node* get_min_node();
	sv_graph_node* get_max_node();
	void set_contig(string s);
	string get_contig();
	void add(sv_graph_node *n);
	int size();
	vector<sv_graph_node*>* get_nodes();
	inline bool operator<(sv_graph_component &other);
	static bool compare_sv_graph_components(
			sv_graph_component *first, 
			sv_graph_component *second);
private:
	vector<sv_graph_node*> *nodes;
	sv_graph_node *min_node, *max_node;
	string contig;
};

#endif
