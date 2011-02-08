#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

#include "seq_file/seq_file.h"


int main (int argc, char** argv) {

	if (argc < 4) {
		cerr << "usage:\tseq_file_test <start> <end> <file>" << endl;
		return 1;
	}

	int start = atoi(argv[1]), end = atoi(argv[2]);

	seq_file f(argv[3]);

	cout << f.get_seq(start, end) << endl;

	return 0;
}
