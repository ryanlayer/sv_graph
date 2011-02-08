class sv_graph;
#ifndef __SV_GRAPH__
#define __SV_GRAPH__

#include <string>
#include <vector>
#include <fstream>
#include "sv_graph_node.h"
using namespace std;


class sv_graph{
public:
	sv_graph();
	//~sv_graph();
	void add_pair(string first, string second);
	void add_pair(vector<string> *first, vector<string> *second);
	vector<sv_graph_node*>* get_nodes();
	int get_min_offset();
	int get_max_offset();
	void import_nodes(FILE *file);
	void import_al(ifstream *file);
	int get_nm_bin( vector<vector<sv_graph_node*> > **nm_bins, int bin_size);
	int get_nm_bin( vector<vector<sv_graph_node*> > **nm_bins, 
					int bin_size,
					int score);
	int get_nm_pairs( vector<sv_graph_node*> **nm_pairs );
private:
	vector<sv_graph_node*> *nodes;
	int max_offset;
	int min_offset;
};

#endif
