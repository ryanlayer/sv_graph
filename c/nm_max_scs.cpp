#include <iostream>
#include <fstream>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <gsl/gsl_statistics_int.h>

using namespace std;

#include "sv_graph/sv_graph_node.h"
#include "sv_graph/sv_graph.h"

int main(int argc, char** argv) {

	if (argc < 3) {
		cout << "usage:\tnm_max_scs <bin size> <al file>" << endl;
		return 1;
	}

	sv_graph graph;

	ifstream file(argv[2]);
	if (file.is_open()) {
		graph.import_al(&file);
	} else {
		cerr << "Unable to open " << argv[2] << endl;
		return 1;
	}

	int bin_size = atoi(argv[1]);
	int min_o = graph.get_min_offset();
	int max_o = graph.get_max_offset();
	int num_bins = (max_o - min_o) / bin_size;
	
	vector<sv_graph_node*> *nm_bins = new vector<sv_graph_node*>[num_bins];

	vector<sv_graph_node> nodes = graph.get_nodes();
	vector<sv_graph_node>::const_iterator node_iter;

	for(node_iter = nodes.begin(); node_iter != nodes.end(); node_iter++) {
		sv_graph_node c = *node_iter;
		if ( (c.get_type() == sv_graph_node::U)  &&
				(c.get_pair()->get_type() == sv_graph_node::NM) ) {
			int bin_id = (c.get_offset() - min_o) / bin_size;
			nm_bins[bin_id].push_back(&c);
			cout << bin_id << "\t" <<
					c.get_offset() << "\t" << 
					c.get_pair()->get_seq() <<endl;
		}

	}

	int *sample = new int[num_bins];
	int bin_id, bin_max = 0;
	for (int i = 0; i < num_bins; i++) {
		sample[i] =  nm_bins[i].size();
		if (bin_max < sample[i]) {
			bin_max = sample[i];
			bin_id = i;
		}
	}

	cout << "id:" << bin_id << "\t" << bin_max << endl;

	vector<sv_graph_node*>::const_iterator nm_iter;
	for(nm_iter = nm_bins[bin_id].begin(); 
			nm_iter != nm_bins[bin_id].end(); nm_iter++) {
		cout << (*nm_iter)->get_offset() << endl;
	}

	/*
	double mean = gsl_stats_int_mean(sample, 1, num_bins);
	double sd = gsl_stats_int_sd(sample, 1, num_bins);


	for (int i = 0; i < num_bins; i++) {
		if (sample[i] >= mean + 0.25 * sd) {
			cout << i*bin_size + min_o << "\t" << sample[i] << endl;
		}
	}
	*/


	return 0;
}
