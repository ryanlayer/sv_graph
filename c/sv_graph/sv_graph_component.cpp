#include <vector>

using namespace std;

#include "sv_graph_component.h" 
#include "sv_graph_node.h" 

sv_graph_component::
sv_graph_component()
{
	nodes = new vector<sv_graph_node*>();
	min_node = NULL;
	max_node = NULL;
}

/*
 * This is the NM node in the pair, to get the offset you must get the pair of
 * the node
 */
sv_graph_node*
sv_graph_component::
get_min_node()
{
	return min_node;
}

/*
 * This is the NM node in the pair, to get the offset you must get the pair of
 * the node
 */
sv_graph_node*
sv_graph_component::
get_max_node()
{
	return max_node;
}

/*
 * We are assuming that the NM node is being added, not the U node
 */
void
sv_graph_component::
add(sv_graph_node *n)
{
	nodes->push_back(n);

	if (min_node == NULL) {
		min_node = n;
		max_node = n;
		return;
	} 
	
	sv_graph_node *n_p = n->get_pair();

	if (n_p != NULL) {
		sv_graph_node *min_p = min_node->get_pair();
		sv_graph_node *max_p = max_node->get_pair();
		
		if ( n_p->get_offset() < min_p->get_offset() )
			min_node = n;

		if ( n_p->get_offset() > max_p->get_offset() )
			max_node = n;
	}
}

int
sv_graph_component::
size()
{
	return nodes->size();
}

vector<sv_graph_node*>*
sv_graph_component::
get_nodes()
{
	return nodes;
}

bool
sv_graph_component::
operator<(sv_graph_component &other)
{
	//return a.get_weight() < b.get_weight();
	return min_node < other.get_min_node();
}

bool
sv_graph_component::
compare_sv_graph_components(sv_graph_component *first, 
							sv_graph_component *second)
{

	return  first->get_min_node()->get_pair()->get_offset() <
			second->get_min_node()->get_pair()->get_offset();
}

void
sv_graph_component::
set_contig(string s) {
	contig = s;
}

string
sv_graph_component::
get_contig() {
	return contig;
}
