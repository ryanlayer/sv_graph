#include <cstdlib>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

#include "novoalign_line.h"

int main(int argc, char ** argv) {

	if (argc < 3) {
		printf("usage:\tnovo_parse_test <novoalign file> <start chr>\n");
		return 1;
	}

	ifstream file(argv[1]);
	if (!file.is_open()) {
		cerr << "Unable to open " << argv[1] << endl;
		return 1;
	}

	char start = argv[2][0];

	map<string,vector<novoalign_line*> > pairs;
	string tmp;
	while ( !file.eof() ) {
		getline(file, tmp);
		novoalign_line *p = new novoalign_line(tmp, start);

		if (p->get_status() == 'U') {
			string id = p->get_read_header();

			stringstream out;
			out << p->get_aligned_offset();
			pairs[ id.substr(0, id.size() - 2) ]
				.push_back( p );

		} else if (p->get_status() == 'N') {
			string id = p->get_read_header();

			pairs[id.substr(0, id.size() - 2)]
				.push_back( p );
		}
	}

	map<string, vector<novoalign_line*> >::iterator it;
	for( it = pairs.begin(); it != pairs.end(); it++ ) {
		if ( (*it).second.size() == 2 ) {
			if ( ((*it).second[0]->get_status() == 'U') ||
					((*it).second[1]->get_status() == 'U') )
				cout << *((*it).second[0]) << ":" << *((*it).second[1]) << endl;
		}
	}
	
	return 0;
}
