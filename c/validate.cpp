#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

#include "seq_file/seq_file.h"
#include "sv_graph/sv_graph.h"
#include "sv_graph/sv_graph_node.h"
#include "nm_graph.h"
#include "sfx/sfx_tree.h"

const int MAXLINE=256;

int main (int argc, char** argv) {

	if (argc < 3) {
		cerr << "usage:\tvalidatae <bed file> <seq file> <al file>" << endl;
		return 1;
	}
	ifstream inFile (argv[1]);
	seq_file f(argv[2]);
	char *al_file = argv[3];

    sv_graph *graph = new sv_graph();

	cerr << "Reading " << al_file << endl;
	ifstream file(al_file);
	if (file.is_open()) {
		graph->import_al(&file);
	} else {
		cerr << "Unable to open " << al_file << endl;
		return 1;
	}
	vector<sv_graph_node*> *nm_pairs = NULL;
	graph->get_nm_pairs(&nm_pairs);

	vector<sv_graph_node*>::const_iterator i;
	vector<u_nm_pair*> *u_nm_pairs = new vector<u_nm_pair*>();

	for(i = nm_pairs->begin(); i != nm_pairs->end(); i++) {
		u_nm_pair *n = new u_nm_pair();
		n->first.first = (*i)->get_offset();
		n->first.second = (*i)->get_offset();
		n->second = (*i)->get_pair()->get_seq();
		u_nm_pairs->push_back( n );
		delete (*i);
	}
	delete nm_pairs;
	delete graph;


	char line[MAXLINE];

	char *chr;
	int start, end;
	string last = "";

	while ( inFile.getline(line, MAXLINE) ) {
		chr = strtok(line, "\t");
		start = atoi(strtok(NULL, "\t"));
		end = atoi(strtok(NULL, "\t"));

		string c = f.get_seq(start, end);

		int x = 0, y = 0;
		string q = "";

		if (c.length() >= 30) {
			q = c.substr(c.length() - 15, 14) + c.substr(0, 15);
			//string q = c.substr(c.length() - 10, 9) + c.substr(0, 10);

			//cout << q << endl;
	
			vector<u_nm_pair*>::const_iterator j;
			size_t found;
			for (j = u_nm_pairs->begin(); j != u_nm_pairs->end(); j++) {
				found = (*j)->second.find(q);
				if (found != string::npos) {
					++x;
					if (	( (*j)->first.first >= start ) &&
							( (*j)->first.first <= end) )
						++y;
				}
			}
		}

		sfx_tree tree_l( last );
		sfx_tree tree_q( q );
		int o_l = tree_l.overlap( q );
		int o_q = tree_q.overlap( last );
		last = q;

		cout << chr << "\t" << start << "\t" << end << "\t" << 
				x << "\t" << y << "\t" << q << "\t" << o_l << 
				"\t" << o_q <<endl;
	}

	inFile.close();

	return 0;
}
