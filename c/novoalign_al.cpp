#include <cstdlib>
#include <limits.h>
#include <cstring>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

#include "novoalign_line/novoalign_line.h"

int main(int argc, char ** argv) {

	if (argc < 3) {
		printf("usage:\tnovoalign_al <novoalign file1,file2,..> <start chr> "
				"<map file>\n");
		return 1;
	}

	map<string, string> name_map;

	/* Parse the chrm name map file if available */
	if (argc == 4) {
		ifstream map_file(argv[3]);
		if (!map_file.is_open()) {
			cerr << "Unable to open " << argv[3] << endl;
			return 1;
		}
		char str_a[LINE_MAX], str_b[LINE_MAX];
		map_file.getline(str_a, LINE_MAX);
		while (!map_file.eof()) {
			map_file.getline(str_a, LINE_MAX, '\t');
			map_file.getline(str_b, LINE_MAX, '\t');
			name_map[ string(str_a) ] = string(str_b);
			map_file.getline(str_b, LINE_MAX);
		}
		map_file.close();
	}


	char start = argv[2][0];

	map<string,vector<novoalign_line*> > pairs;
	string tmp;
	char *p, *a = argv[1];
	p = strtok(a, ",");
	while (p) {
		ifstream file(p);
		if (!file.is_open()) {
			cerr << "Unable to open " << p << endl;
			return 1;
		}

		while ( !file.eof() ) {
			getline(file, tmp);
			//cerr << tmp << endl;
			//novoalign_line *l = new novoalign_line(tmp, start);
			novoalign_line *l = new novoalign_line(tmp, start, &name_map);
			//cerr << *l  << endl;


			if ( (l->get_status() == 'U') || (l->get_status() == 'N') ) {
				string id = l->get_read_header();

				pairs[ id.substr(0, id.size() - 2) ]
					.push_back( l );
			}
		}

		p = strtok(NULL, ",");
	}

	map<string, ofstream*> out_files;
	map<string, vector<novoalign_line*> >::iterator it;
	for( it = pairs.begin(); it != pairs.end(); it++ ) {

		if ( (*it).second.size() == 2 ) {
			string al_0, al_1, s;
			stringstream ss;

			ss << *((*it).second[0]) << "\t" << *((*it).second[1]) << endl;
			s = ss.str();

			if ( (*it).second[0]->get_status() == 'U' ) 
				al_0 = (*it).second[0]->get_aligned_sequence();

			if ( (*it).second[1]->get_status() == 'U' ) 
				al_1 = (*it).second[1]->get_aligned_sequence();

			// Neither end can be placed
			if ( al_0.empty() && al_1.empty() )
				;
			// They are equal or only the second can be placed
			else if (  al_0.empty() || ( al_0.compare(al_1) == 0 ) ) { 
				if ( out_files[al_1]  == NULL )
					out_files[al_1] = new ofstream( al_1.c_str() );
				*out_files[al_1] << s;
			} else if ( al_1.empty() ) { // Only the first can be placed
				if ( out_files[al_0]  == NULL )
					out_files[al_0] = new ofstream( al_0.c_str() );
				*out_files[al_0] << s;
			} else { // Both can be placed
				if ( out_files[al_1]  == NULL )
					out_files[al_1] = new ofstream( al_1.c_str() );
				if ( out_files[al_0]  == NULL )
					out_files[al_0] = new ofstream( al_0.c_str() );
				*out_files[al_0] << s;
				*out_files[al_1] << s;
			}
		}
	}

	map<string, ofstream*>::iterator of_it;
	for( of_it = out_files.begin(); of_it != out_files.end(); of_it++ ) {
		(*of_it).second->close();
	}
	
	return 0;
}
