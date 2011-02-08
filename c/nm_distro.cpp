#include <iostream>
#include <fstream>
#include <cstdlib>
#include <fstream>
#include <gsl/gsl_statistics_int.h>

using namespace std;

#include "sv_graph/sv_graph_node.h"
#include "sv_graph/sv_graph.h"

int main(int argc, char** argv) {

	if (argc < 3) {
		cout << "usage:\tnm_map <bin size> <threshold> <al file>" << endl;
		return 1;
	}

	sv_graph *graph = new sv_graph();

	ifstream file(argv[3]);
	if (file.is_open()) {
		graph->import_al(&file);
	} else {
		cerr << "Unable to open " << argv[2] << endl;
		return 1;
	}

	int bin_size = atoi(argv[1]);
	int T = atoi(argv[2]);
	int min_o = graph->get_min_offset();

	vector<vector<sv_graph_node*> > *nm_bins = NULL;
	int quality_score = 0;
	int num_nm_bins = graph->get_nm_bin(&nm_bins, bin_size, quality_score);
	//int num_nm_bins = graph->get_nm_bin(&nm_bins, bin_size);


	for (int i = 0; i < num_nm_bins; i++) {
		int s =(*nm_bins)[i].size(); 
		if ( s >= T) 
			cout << i*bin_size + min_o << "\t" << s << endl;
	}

	return 0;
}
