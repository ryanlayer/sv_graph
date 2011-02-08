#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <limits.h>
#include <algorithm>
#include <cstdio>
#include <sstream>

using namespace std;

#include "sv_graph_node.h"
#include "sv_graph.h"

sv_graph::sv_graph() {
	min_offset = numeric_limits<int>::max();
	max_offset = numeric_limits<int>::min();
	nodes = new vector<sv_graph_node*>;
}

void 
sv_graph::add_pair(vector<string> *first, vector<string> *second) 
{

	sv_graph_node *f = new sv_graph_node(first);
	sv_graph_node *s = new sv_graph_node(second);
	//sv_graph_node *f = new sv_graph_node(first);
	//sv_graph_node *s = new sv_graph_node(second);

	f->join_pair(s);
	s->join_pair(f);

	nodes->push_back(f);
	nodes->push_back(s);

	if (f->get_type() == sv_graph_node::U) {
		min_offset = min(min_offset, f->get_offset());
		max_offset = max(max_offset, f->get_offset());
	}

	if (s->get_type() == sv_graph_node::U) {
		min_offset = min(min_offset, s->get_offset());
		max_offset = max(max_offset, s->get_offset());
	}
}


void 
sv_graph::add_pair(string first, string second) 
{

	sv_graph_node *f = new sv_graph_node(first);
	sv_graph_node *s = new sv_graph_node(second);

	f->join_pair(s);
	s->join_pair(f);

	nodes->push_back(f);
	nodes->push_back(s);

	if (f->get_type() == sv_graph_node::U) {
		min_offset = min(min_offset, f->get_offset());
		max_offset = max(max_offset, f->get_offset());
	}

	if (s->get_type() == sv_graph_node::U) {
		min_offset = min(min_offset, s->get_offset());
		max_offset = max(max_offset, s->get_offset());
	}
}

vector<sv_graph_node*>*
sv_graph::get_nodes() 
{
	return nodes;
}

int
sv_graph::get_min_offset() 
{
	return min_offset;
}

int
sv_graph::get_max_offset() 
{
	return max_offset;
}


void
sv_graph::import_nodes(FILE *file) 
{

	int i, f = 0, s = 0, w = 1;
	char first[LINE_MAX];
	char second[LINE_MAX];

	fgets(first, LINE_MAX, stdin);

	while( (i = fgetc(stdin)) != EOF ) {
		if( (char)i == ',' ) {
			first[f] = '\0';
			f = 0;
			w = 2;
		} else if ( ((char)i == ';') || 
					((char)i == '\n') ||
					((char)i == '\0') ) {
			second[s] = '\0';
			s = 0;
			w = 1;
			add_pair(string(first), string(second));
		} else if (w == 1)
			first[f++] = (char)i;
		else 
			second[s++] = (char)i;
	}

}


/*
 * U,18964,>gi|6226515|ref|NC_001224.1|,0,35;U,18367,>gi|6226515|ref|NC_001224.1|,0,53
 * U,43125,>gi|6226515|ref|NC_001224.1|,0,25;N,ATCAATTAAAAAATAAAACA,.
 */
void
sv_graph::import_al(ifstream *file)
{
	string line;
	string tmp;
	while ( !file->eof() ) {
		getline (*file, line);

		if (! line.empty() ) {
			stringstream ends(line);
			string first_end, second_end;

			getline(ends, first_end, '\t');
			getline(ends, second_end, '\t');

			stringstream split_first (first_end);
			stringstream split_second (second_end);

			vector<string> first_fields, second_fields;
				
			while ( getline(split_first, tmp, ',') )
				first_fields.push_back(tmp);

			while ( getline(split_second, tmp, ',') )
				second_fields.push_back(tmp);

			//add_pair(first_fields[1], second_fields[1]);
			//cout << line << endl;
			add_pair(&first_fields, &second_fields);
		}
	}
}

/*
 * Return a vector or nodes that uniquely mapped and have a pair that does not
 * map
 */
int
sv_graph::get_nm_pairs( vector<sv_graph_node*> **nm_pairs )
{

	*nm_pairs = new vector<sv_graph_node*> ();
	int num_nm_pairs = 0;

	vector<sv_graph_node*>::const_iterator node_iter;

	for(node_iter = nodes->begin(); node_iter != nodes->end(); node_iter++) {
		sv_graph_node *c = *node_iter;
		if ( (c->get_type() == sv_graph_node::U)  &&
			 (c->get_pair()->get_type() == sv_graph_node::NM) ) {
			(**nm_pairs).push_back(c);
			++num_nm_pairs;
		}
	}

	return num_nm_pairs;
}





/*
 * Return bins that contain U nodes that are paired with NM nodes
 */
//vector<vector<sv_graph_node*> >* sv_graph::get_nm_bin(int bin_size) {
int
sv_graph::get_nm_bin( vector<vector<sv_graph_node*> > **nm_bins, 
					  int bin_size)
{

	int num_bins = (max_offset - min_offset) / bin_size;
	*nm_bins = new vector<vector<sv_graph_node*> >(num_bins);

	vector<sv_graph_node*>::const_iterator node_iter;

	for(node_iter = nodes->begin(); node_iter != nodes->end(); node_iter++) {
		sv_graph_node *c = *node_iter;
		if ( (c->get_type() == sv_graph_node::U)  &&
			 (c->get_pair()->get_type() == sv_graph_node::NM) ) {

			int bin_id = (c->get_offset() - min_offset) / bin_size;
			(**nm_bins)[bin_id].push_back(c);
		}
	}

	return num_bins;
}

/*
 * Return bins that contain U nodes that are paired with NM nodes
 */
int
sv_graph::get_nm_bin( vector<vector<sv_graph_node*> > **nm_bins, 
					  int bin_size,
					  int score)
{

	int num_bins = (max_offset - min_offset) / bin_size;
	*nm_bins = new vector<vector<sv_graph_node*> >(num_bins);

	vector<sv_graph_node*>::const_iterator node_iter;

	for(node_iter = nodes->begin(); node_iter != nodes->end(); node_iter++) {
		sv_graph_node *c = *node_iter;
		if ( (c->get_type() == sv_graph_node::U)  &&
			 (c->get_score() <= score)  &&
			 (c->get_pair()->get_type() == sv_graph_node::NM)) {

			int bin_id = (c->get_offset() - min_offset) / bin_size;
			(**nm_bins)[bin_id].push_back(c);
		}
	}

	return num_bins;
}
