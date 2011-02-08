#include <iostream>                  // for std::cout
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <map>
#include <string> 
#include <string.h> 
#include <stdlib.h> 

using namespace std;

#include "sv_graph/sv_graph_node.h"
#include "sv_graph/sv_graph.h"
#include "sv_graph/sv_edge.h"
#include "sfxtree/sfxtree.h"

int main(int argc,char** argv) {

	if (argc < 4) {
		cout << "usage:\tablink <bin size> <threshold> <al file>" << endl;
		return 1;
	}

	int bin_size = atoi(argv[1]);
	int T = atoi(argv[2]);

	sv_graph *graph = new sv_graph();

	ifstream file(argv[3]);
	if (file.is_open()) {
		graph->import_al(&file);
	} else {
		cerr << "Unable to open " << argv[2] << endl;
		return 1;
	}

	vector<sv_graph_node*> *nodes = graph->get_nodes();

	vector<sv_graph_node*>::iterator i;

	for (i = nodes->begin(); i != nodes->end(); i++) {
		if ( ( (*i)->get_type() == sv_graph_node::U ) &&
			 ( (*i)->get_pair()->get_type() == sv_graph_node::U ) )
			cout << (*i)->get_offset() << "\t" << 
					(*i)->get_pair()->get_offset() << "\t" <<
					( abs((*i)->get_offset() - (*i)->get_pair()->get_offset()) ) 
					<< endl;
	}
}
