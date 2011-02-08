#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <limits.h>

using namespace std;


string trim(string str)
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

	str_.erase(str_.length() - count, count);

	return str_;
}

int main (int argc, char** argv) {
	int length;
	char *buffer = new char[LINE_MAX];

	int start = atoi(argv[1]), end = atoi(argv[2]);

	/* 
	 * each line ends with a '\n', so we need add those additional chars to our
	 * start possition
	 */
	int seek_dist = (start / 70) * 71 + (start % 70) - 1;
	int most_breaks = ( (end - start + 1) / 70) + 1;

	ifstream is;
	is.open (argv[3]);

	is.getline(buffer, LINE_MAX);

	int  data_start = is.tellg();

	//is.seekg (data_start + start + end_lines, ios::beg);
	is.seekg (data_start + seek_dist, ios::beg);
	is.read (buffer, end - start + 1 + most_breaks);
	buffer[end - start + 1 + most_breaks] = '\0';

	cout << trim(string(buffer)) << endl;

	delete[] buffer;
	return 0;
}
