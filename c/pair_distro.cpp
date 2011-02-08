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

	if (argc < 2) {
		cout << "usage:\tpair_distro <al file>" << endl;
		return 1;
	}

	sv_graph *graph = new sv_graph();

	ifstream file(argv[1]);
	if (file.is_open()) {
		graph->import_al(&file);
	} else {
		cerr << "Unable to open " << argv[1] << endl;
		return 1;
	}

	vector<sv_graph_node*> *nodes = graph->get_nodes();

	vector<sv_graph_node*>::iterator i;

	for (i = nodes->begin(); i != nodes->end(); i++) {
		if ( ( (*i)->get_type() == sv_graph_node::U ) &&
			 ( (*i)->get_pair()->get_type() == sv_graph_node::U ) &&
			 ( (*i)->get_score() == 0 ) &&
			 ( (*i)->get_pair()->get_score() == 0 ) ) {

			cout << (*i)->get_offset() << "\t" << 
					(*i)->get_chr() << "\t" << 
					(*i)->get_score() << "\t" << 
					(*i)->get_pair()->get_offset() << "\t" <<
					(*i)->get_pair()->get_chr() << "\t" << 
					(*i)->get_pair()->get_score() << "\t" << 
					( abs((*i)->get_offset() - 
						  (*i)->get_pair()->get_offset()) ) 
					<< endl;
		} else if ( ( (*i)->get_type() == sv_graph_node::U ) &&
					( (*i)->get_pair()->get_type() == sv_graph_node::NM ) &&
					( (*i)->get_score() == 0 ) ) {

			cout << (*i)->get_offset() << "\t" << 
					(*i)->get_chr() << "\t" << 
					(*i)->get_score() << "\t" << 
					(*i)->get_pair()->get_seq() << "\t" <<
					endl;
		}
	}
}
