#include <iostream> 

using namespace std;

#include "sfxtree/sfxtree.h"

int main(int argc, char** argv) {
	sfxtree tree(argv[1]);
	tree.print_tree();
	cout<< tree.max_suffix(argv[2]) << endl;
	return 0;
}
