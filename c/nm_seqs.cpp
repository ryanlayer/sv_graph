#include <iostream>
#include <fstream>
#include <cstdlib>
#include <fstream>

using namespace std;

#include "sv_graph/sv_graph_node.h"
#include "sv_graph/sv_graph.h"

int main(int argc, char** argv) {

	if (argc < 4) {
		cout << "usage:\tnm_seqs <bin size> <threshold> <al file>" << endl;
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
	//int min_o = graph->get_min_offset();
	//int max_o = graph->get_max_offset();
	//int num_bins = (max_o - min_o) / bin_size;
	
	vector<vector<sv_graph_node*> > *nm_bins = NULL;
	//int num_nm_bins = graph->get_nm_bin(&nm_bins, bin_size);
	int quality_score = 0;
	int num_nm_bins = graph->get_nm_bin(&nm_bins, bin_size, quality_score);

	int *sample = new int[num_nm_bins];

	for (int i = 0; i < num_nm_bins; i++)
		sample[i] =  (*nm_bins)[i].size();

	for (int i = 0; i < num_nm_bins; i++) {
		if (sample[i] >= T) {
			vector<sv_graph_node*>::const_iterator nm_iter;
			for (nm_iter = (*nm_bins)[i].begin(); 
					nm_iter != (*nm_bins)[i].end(); nm_iter++) {
				sv_graph_node *c = *nm_iter;
				cout << c->get_offset() << "\t" << 
						c->get_pair()->get_seq() << "\t" <<
						c->get_score() << endl;
			}

			cout << endl;

		}
	}

	return 0;
}
