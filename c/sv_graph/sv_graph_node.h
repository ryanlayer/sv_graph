class sv_edge;
#ifndef __SV_GRAPH_NODE__
#define __SV_GRAPH_NODE__

#include <string>
#include <vector>
using namespace std;

#include "sv_edge.h"
#include "sv_graph_component.h"

class sv_graph_node {
public:
	static int NM, U, white, black, gray, green;
	sv_graph_node(string val);
	sv_graph_node(vector<string> *val);
	int get_color();
	void set_color(int c);
	int get_type();
	int get_offset();
	int get_score();
	int get_quality();
	string get_chrm();
	string get_seq();
	sv_graph_node* get_pair();
	void join_pair(sv_graph_node *p);
	void add_edge(sv_graph_node *p, int w);
	void add_in_edge(sv_edge *edge);
	vector<sv_edge*>* get_out_edges();
	vector<sv_edge*>* get_in_edges();
	void print_out_edges();
	string get_chr();
	//void get_connected_nodes(vector<sv_graph_node*> *vs);
	void get_connected_nodes(sv_graph_component *c);
	inline bool operator<(sv_graph_node &other);
	static bool compare_sv_graph_nodes(sv_graph_node *first, 
			sv_graph_node *second);
private:
	int color;
	int type;
	int offset;
	int score;
	int quality;
	string seq;
	string chr;
	sv_graph_node *pair;
	vector<sv_edge*> *in, *out;
};

#endif
