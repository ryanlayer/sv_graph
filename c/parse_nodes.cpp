#include <iostream>
#include <cstdlib>
#include <fstream>
#include <gsl/gsl_statistics_int.h>

using namespace std;

#include "sv_graph/sv_graph_node.h"
#include "sv_graph/sv_graph.h"

int main(int argc, char** argv) {

	if (argc < 2) {
		cout << "usage:\tparse_nodes <bin size>" << endl;
		return 1;
	}

	sv_graph graph;
	graph.import_nodes(stdin);

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
		}

	}

	int *sample = new int[num_bins];
	for (int i = 0; i < num_bins; i++) {
		sample[i] =  nm_bins[i].size();
		cout << i*bin_size + min_o << "\t" << sample[i] << endl;
	}

	double mean = gsl_stats_int_mean(sample, 1, num_bins);
	double sd = gsl_stats_int_sd(sample, 1, num_bins);

	cout << mean << "\t" << sd << endl;

	for (int i = 0; i < num_bins; i++) {
		if (sample[i] >= mean + 0.25 * sd) {
			cout << i*bin_size + min_o << "\t" << sample[i] << endl;
		}
	}

	return 0;
}
