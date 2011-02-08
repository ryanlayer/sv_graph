#ifndef __NM_GRAPH_H__
#define __NM_GRAPH_H__

#include "sv_graph/sv_graph_node.h"
//#include "sv_graph/sv_graph.h"
//#include "sv_graph/sv_edge.h"
//#include "sv_graph/sv_graph_component.h"
////#include "sfxtree/sfxtree.h"
//#include "sfx/sfx_tree.h"
//#include "seq_file/seq_file.h"
//#include "nm_graph.h"



typedef pair<pair<int, int>, string> u_nm_pair;

std::string rev_comp(std::string s);

void search(sv_graph_node *v, string *contig, int *w);

void sfx_join_sv_graph_nodes( vector<sv_graph_node*> *nodes, int threshold );

void sfx_join_nodes( vector<u_nm_pair*> *nodes, int threshold,
	   vector<u_nm_pair*> **result);

void sfx_connect_nodes( vector<sv_graph_node*> *nodes, int threshold);

#endif
