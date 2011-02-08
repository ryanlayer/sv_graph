#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <limits.h>

using namespace std;

#include "seq_file.h"

string
seq_file::
trim(string str, int most_breaks)
{
	vector<int> dels;
	string str_ = str;
	int count = 0;
	string::size_type word_pos(0);
	while ( word_pos != string::npos ) {
		word_pos = str.find("\n", word_pos);

		if ( word_pos != string::npos ) {
			dels.push_back(word_pos);
			word_pos++;
			++count;
		}

	}

	vector<int>::reverse_iterator i;
	for (i = dels.rbegin(); i != dels.rend(); i++) {
		str_.erase((*i), 1);
	}

	// most_breaks is the extra number of characters read in anticipation of
	// there being a '\n' character in the string, at most, there will be
	// most_breaks for a string of a given size, so most_breaks extra
	// characters are read.  above we calculate exactly how many '\n' there
	// are, and remove them, we then need to trim the extra characters
	
	int extra = most_breaks - count;

	str_.erase(str_.length() - extra, extra);

	return str_;
}

seq_file::
~seq_file() {
	is.close();
	delete buffer;
}

seq_file::
seq_file(string file_name) {
	buffer = new char[LINE_MAX];
	is.open (file_name.c_str());
	is.getline(buffer, LINE_MAX);
	data_start = is.tellg();

}

string
seq_file::
get_seq(int start, int end){
	int seek_dist = (start / 70) * 71 + (start % 70) - 1;
	int most_breaks = ( (end - start + 1) / 70) + 1;
	char b[end - start + 1 + most_breaks];

	is.seekg (data_start + seek_dist, ios::beg);
	is.read (b, end - start + 1 + most_breaks);
	b[end - start + 1 + most_breaks] = '\0';

	return trim(string(b), most_breaks);
}
