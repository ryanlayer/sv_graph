#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <cstdlib>

using namespace std;

#include "novoalign_line.h"

novoalign_line::novoalign_line( string line, 
								char start, 
								map<string, string> *m )
{
	stringstream ss(line);
	vector<string> fields;

	if (line[0] == start) {
		string tmp;
		while( getline(ss, tmp, '\t') )
			fields.push_back(tmp);

		if ( fields[4].compare("NM") == 0 ) {
			read_header = fields[0];
			read_type = fields[1][0];
			read_sequence = fields[2];
			base_qualities = fields[3];
			status = 'N';
		} else if ( fields[4].compare("U") == 0 ) {
			read_header = fields[0];
			read_type = fields[1][0];
			read_sequence = fields[2];
			base_qualities = fields[3];
			status = 'U';
			alignment_score = atoi(fields[5].c_str());
			alignment_quality= atoi(fields[6].c_str());
			aligned_sequence= (*m)[fields[7]];
			aligned_offset= atoi(fields[8].c_str());
			strand = fields[9][0];
		} else {
			status = 'X';
		}
	} else {
		status = 'X';
	}
}

novoalign_line::novoalign_line( string line, 
								char start )
{
	stringstream ss(line);
	vector<string> fields;

	if (line[0] == start) {
		string tmp;
		while( getline(ss, tmp, '\t') )
			fields.push_back(tmp);

		if ( fields[4].compare("NM") == 0 ) {
			read_header = fields[0];
			read_type = fields[1][0];
			read_sequence = fields[2];
			base_qualities = fields[3];
			status = 'N';
		} else if ( fields[4].compare("U") == 0 ) {
			read_header = fields[0];
			read_type = fields[1][0];
			read_sequence = fields[2];
			base_qualities = fields[3];
			status = 'U';
			alignment_score = atoi(fields[5].c_str());
			alignment_quality= atoi(fields[6].c_str());
			aligned_sequence= fields[7];
			aligned_offset= atoi(fields[8].c_str());
			strand = fields[9][0];
		} else {
			status = 'X';
		}
	} else {
		status = 'X';
	}
}

ostream& operator<<( ostream& os,
					 const novoalign_line& n) 
{
	
	if (n.status == 'U') {
		os << n.status << "," <<
			n.aligned_offset << "," <<
			n.aligned_sequence << "," <<
			n.alignment_score << "," <<
			n.alignment_quality;
	} else if (n.status == 'N') {
		os << n.status << "," <<
			n.read_sequence << "," <<
			n.base_qualities;
	} else {
		os << "X";
	}

	return os;
}



//{{{ getters
string novoalign_line::get_read_header(){ return read_header; }
char novoalign_line::get_read_type(){ return read_type; }
string novoalign_line::get_read_sequence(){ return read_sequence; }
string novoalign_line::get_base_qualities(){ return base_qualities; }
char novoalign_line::get_status(){ return status; }
int novoalign_line::get_alignment_score(){ return alignment_score; }
int novoalign_line::get_alignment_quality(){ return alignment_quality; }
string novoalign_line::get_aligned_sequence(){ return aligned_sequence; }
int novoalign_line::get_aligned_offset(){ return aligned_offset; }
char novoalign_line::get_strand(){ return strand; }
//}}}
