#include <iostream> 

using namespace std;

#include "sfx/sfx_tree.h"

int main(int argc, char** argv) {
	sfx_tree tree( argv[1]);
	cout<< tree.overlap(argv[2]) << endl;

	int i, j;
	tree.search(argv[2], i, j);

	cout << i << "\t" << j << endl;

	return 0;
}
