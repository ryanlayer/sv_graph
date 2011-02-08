#include <iostream>                  // for std::cout
#include <queue>
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <map>
#include <string> 
#include <string.h> 
#include <stdlib.h> 
#include <limits.h> 
#include <boost/shared_ptr.hpp>

using namespace std;

#include "sv_graph/sv_graph_node.h"
#include "sv_graph/sv_graph.h"
#include "sv_graph/sv_edge.h"
#include "sv_graph/sv_graph_component.h"
//#include "sfxtree/sfxtree.h"
#include "sfx/sfx_tree.h"
#include "seq_file/seq_file.h"
#include "nm_graph.h"

bool compare_u_nm_pair(u_nm_pair *a, u_nm_pair *b)
{
	return  a->first.first < b->first.first;
}


//{{{ std::string rev_comp(std::string s) {
std::string rev_comp(std::string s) {
	std::string::iterator it;
	std::string r = "";
	for( it = s.begin(); it != s.end(); it++) {
		if (*it == 'A')
			r = "T" + r;
		else if (*it == 'T')
			r = "A" + r;
		else if (*it == 'G')
			r = "C" + r;
		else  // C
			r = "G" + r;
	}

	return r;
}
//}}}

//{{{ void search(sv_graph_node *v, string *contig, int *w) {
void search(sv_graph_node *v, string *contig, int *w) {

	v->set_color(sv_graph_node::black);

	vector<sv_edge*> *edges = v->get_out_edges();
	vector<sv_edge*>::const_iterator i;

	int max_child_weight = -1;
	sv_graph_node *t = NULL, *u = NULL;

	for (i = edges->begin(); i != edges->end(); i++) {
		int u_w = (*i)->get_weight();
		u = (*i)->get_target();
		
		/*
		if ( u_w == v->get_seq().length() )
			u->set_color(sv_graph_node::black);
			*/

		if (    ( u->get_color() != sv_graph_node::black ) &&
				( u_w > max_child_weight ) ) {
			max_child_weight = u_w;
			t = u;
		}
	}

	if ( max_child_weight > -1 ) {
		string s = t->get_seq();
		*w = *w + max_child_weight;
		*contig += s.substr(max_child_weight, s.length() - max_child_weight);
		search(t, contig, w);
	}

	v->set_color(sv_graph_node::white);
}
//}}}

//{{{ void sfx_join_sv_graph_nodes( vector<sv_graph_node*> *nodes, int threshold ) {
void sfx_join_sv_graph_nodes( vector<sv_graph_node*> *nodes, int threshold,
	   vector<u_nm_pair*> **result	) {

	vector<u_nm_pair*> *u_nm_pairs = new vector<u_nm_pair*>();

	cerr << nodes->size() << "\t";

	// Convert the sv_graph_nodes to u_nm_pair nodes
	vector<sv_graph_node*>::const_iterator node_iter;
	for (node_iter = nodes->begin(); node_iter != nodes->end(); node_iter++) {
		u_nm_pair *new_pair = new u_nm_pair();
	
		new_pair->first.first = (*node_iter)->get_offset();
		new_pair->first.second = (*node_iter)->get_offset();
		new_pair->second = (*node_iter)->get_pair()->get_seq();

		u_nm_pairs->push_back(new_pair);

		//delete (*node_iter)->get_pair();
		//delete (*node_iter);
	}

	vector <u_nm_pair*> *curr, *next;

	curr = u_nm_pairs;

	sfx_join_nodes( curr, threshold, &next);

	while (curr->size() > next->size()) {
		cerr << curr->size() << "\t";
		delete curr;
		curr = next;
		sfx_join_nodes( curr, threshold, &next);
	}
	cerr << curr->size() << "\n";

	delete curr;

	/*
	cout << next->size() << endl;
	vector <u_nm_pair*>::const_iterator i;
	for (i = next->begin(); i != next->end(); i++)
		cout << (*i)->first.first << "\t" <<
				(*i)->first.second << "\t" <<
				(*i)->second <<
				endl;
	*/
	*result = next;
}
//}}}

//{{{void sfx_join_nodes( vector<u_nm_pair*> *nodes, int threshold, 
void sfx_join_nodes( vector<u_nm_pair*> *nodes, int threshold, 
		vector<u_nm_pair*> **result) {

	/*
	 * A group consists of a reference (sfx tree) and an arry of bins.  Each
	 * bin contains a u_nm_pair that overlaps with the refrence sequence at
	 * least as much as defined by the threshold.  All pairs in a bin overlap
	 * the same amount.  Bins are ordered by how much does not overlap bin[0]
	 * completely overlaps bin[1] overlaps all but one char, and so on
	 */
	vector<	pair< 
				sfx_tree*, 
				vector<u_nm_pair*>** >* > *groups = 
			new vector<pair<sfx_tree*, vector<u_nm_pair*>**>*>();

	vector< pair< sfx_tree*, vector<u_nm_pair*>** >* >::
			const_iterator group_iter;
	
	vector<u_nm_pair*>::const_iterator node_iter;

	/*
	 * Loop through the nodes and either place a node in a group, or create a
	 * new groups where the node is used to create the refrence sfx tree
	 */
	for (node_iter = nodes->begin(); node_iter != nodes->end(); node_iter++) {
		bool matched = false;

		// see if it fits in a group
		for (group_iter = groups->begin(); group_iter != groups->end();
				group_iter++) {

			// get the group ref and test the overlap
			sfx_tree *curr_tree = (*group_iter)->first;
			int max_sfx = curr_tree->overlap( (*node_iter)->second.c_str() );

			if (max_sfx >= threshold) {
				//put the node in the group
				
				vector<u_nm_pair*> **new_vector = (*group_iter)->second;
				int len = 
					new_vector[0]->front()->second.length();
				int index = len - max_sfx;
				new_vector[index]->push_back(*node_iter);

				matched = true;
				break;
			}
		}

		// it did not fit in a group
		if (!matched) {
				// create a new array of vectors and initialize them
				vector<u_nm_pair*> **new_vector;
				int len = (*node_iter)->second.length();
				new_vector = new vector<u_nm_pair*>*[len - threshold + 1];

				int i;
				for (i = 0; i < len - threshold + 1; i++)
					new_vector[i] = new vector<u_nm_pair*>;

				// put the node in the 0 bin
				new_vector[0]->push_back( *node_iter );

				// create a tree from the node
				sfx_tree *new_tree = new sfx_tree( 
					(*node_iter)->second.c_str() );

				// put the pair into the group
				pair< sfx_tree*, vector<u_nm_pair*>** > *new_pair = 
					new pair< sfx_tree*, vector<u_nm_pair*>** > (
							new_tree,
							new_vector);
				groups->push_back(new_pair);
		}
	}


	(*result) = new vector<u_nm_pair*>();

	// reduce each group to a single node
	// The process is to first reduce each bin in a group to a set of nodes
	// with a unique seq.  If two nodes in a bin have the same seq, they are
	// joined into a single node that keeps the min/max offset and the seq.  It
	// is possible that a bin has a number of nodes with different seqs.
	for (group_iter = groups->begin(); group_iter != groups->end();
			group_iter++) {

		// The first element in a group is the sfx tree and the second is the
		// bins
		vector<u_nm_pair*> **bins = (*group_iter)->second;

		// The number of bins is based on the length of the reference seq, wich
		// will be the first seq in the first bin
		int len = bins[0]->front()->second.length();
		int i;

		// Use a queue to remove duplicates.  Take all of the elements in the
		// list and put them into a queue, pop the first element and all other
		// elements that equal it, push the unique element back into the list
		for (i = 0; i < len - threshold + 1; i++) {
			queue< u_nm_pair* > *q, *p, *tmp_q;
			q = new queue< u_nm_pair* >();
			p = new queue< u_nm_pair* >();

			vector< u_nm_pair* >::const_iterator j;

			// move elements from the vector to the queue
			for (j = bins[i]->begin(); j != bins[i]->end(); j++)
				q->push(*j);

			bins[i]->clear();

			while (!q->empty()) {
				// pop the first node, then scan the remaining nodes to see if
				// any match, if they do then expand the min/max of new_node,
				// if they dont them push the nodes into p
				u_nm_pair *curr_node = q->front();
				q->pop();

				while (!q->empty()) {
					// see if the seqs are the same
					if ( curr_node->second.compare( q->front()->second) == 0 ) {
						//set the min
						curr_node->first.first = min(
								curr_node->first.first,
								q->front()->first.first );
						//set the max
						curr_node->first.second = max(
								curr_node->first.second,
								q->front()->first.second );
					} else 
						// if they are not the same put the node back in the
						// queue
						p->push(q->front());

					q->pop();
				}

				// now q is empty and p may have nodes, so we just flip them
				tmp_q = q;
				q = p;
				p = tmp_q;

				bins[i]->push_back(curr_node);
			}

			delete q;
		}

		// graph will be a pair of the node (itself a pair) and a vector of // nodes 
		map <	u_nm_pair*, //node 
				vector < // adjacency list 
					pair < //edge 
						int, // weight 
						u_nm_pair* > >* >// adjacent node
				*graph = new map< u_nm_pair*, 
						vector<pair<int, u_nm_pair* > >* > ();

		vector < vector < u_nm_pair* >* >::const_iterator j,k;
		vector < u_nm_pair* >::const_iterator l,m;
		vector < pair < int, u_nm_pair* > > *new_adj_list;

		/*
		 * Each bin contains only unique elemenst.  We need to now reduce tall
		 * of those bins to a single u_nm_pair.  To do so we will create a
		 * graph where nodes are pairs and edges are weighted by their overlap.
		 * We now that higher bins do not overlap with lower bins so we can
		 * skip those comparisons
		 */
		for (int j = 0; j < len - threshold + 1; j++) { //src bin
			//nodes in src bin
			for (l = bins[j]->begin(); l != bins[j]->end(); l++) {
				// create a vector to store adjacnet nodes and weights
				new_adj_list = new vector< pair<int, u_nm_pair*> >();

				sfx_tree tree = sfx_tree( (*l)->second );

				for (int k = j + 1; k < len - threshold + 1; k++) { //src bin
					for (m = bins[k]->begin(); m != bins[k]->end(); m++) {
						int max_sfx = tree.overlap( (*m)->second );
						new_adj_list->push_back( 
								pair<int, u_nm_pair*>(max_sfx, *m) );
					} // **end** nodes in dst bin
				} // *end* dst bin

				// push node and adjacent list into graph
				graph->insert( pair< u_nm_pair*,  
									 vector< pair<int, u_nm_pair*> >* >
									 (*l, new_adj_list));

			} // **end** nodes in src bin
		} // *end* src bin

		map<	u_nm_pair*, 
				vector<pair<int, u_nm_pair* > >* >::const_iterator g;

		// From each node in the graph, we will run a greedy DFS, creating
		// a new u_nm_pair by the u_nm_pairs along the path.  We will pick
		// a single composite u_nm_pair based on the weight of its path,
		// therefore we store the compisites in a pair 
		
		pair<int, u_nm_pair> *max_composite = NULL;

		for (g = graph->begin(); g != graph->end(); g++) {

			pair<int, u_nm_pair> *composite = new pair<int, u_nm_pair>();
			// set min (first.first), max (first.second), and seq (second) 
			composite->second.first.first = (*g).first->first.first;
			composite->second.first.second = (*g).first->first.second;
			composite->second.second = (*g).first->second;
			composite->first = 0;

			//Greedy DFS

			// curr is the set of edges from the current nod
			vector< pair<int, u_nm_pair*> > *curr = (*g).second;

			while ( curr->size() > 0) {
				pair<int, u_nm_pair*> max = curr->front();

				vector< pair<int, u_nm_pair* > >::const_iterator n;

				for (n = curr->begin(); n != curr->end(); n++) {
					if ( (*n).first > max.first)
						max = *n;
				}

				// set min (first.first), max (first.second), seq (second),
				// and set weight
				if ( max.second->first.first < composite->second.first.first)
					composite->second.first.first = max.second->first.first;

				if ( max.second->first.second > composite->second.first.second)
					composite->second.first.second = max.second->first.second;

				composite->second.second.append( max.second->second.substr(
						max.first,
						max.second->second.length() - max.first));

				composite->first = composite->first + max.first;

				curr = graph->at(max.second);
			}

			if ( (max_composite == NULL) || 
				 (max_composite->first < composite->first) )
				max_composite = composite;
			else
				delete composite;
		}

		delete graph;

		(*result)->push_back(&(max_composite->second));
	}

	delete groups;
}
//}}}

//{{{ void sfx_connect_nodes( vector<sv_graph_node*> *nodes, int threshold ) {
void sfx_connect_nodes( vector<sv_graph_node*> *nodes, int threshold ) {

	// N^2 pair matching
	vector<sv_graph_node*>::const_iterator x_iter, y_iter;

	for (x_iter = nodes->begin(); x_iter != nodes->end(); x_iter++) {
		sv_graph_node *x = *x_iter;
		sv_graph_node *xp = x->get_pair();

		//sfxtree x_tree( xp->get_seq() );
		sfx_tree x_tree( xp->get_seq().c_str() );

		for (y_iter = nodes->begin(); y_iter != nodes->end(); y_iter++) {
			if (x_iter != y_iter) {
				sv_graph_node *y = *y_iter;
				sv_graph_node *yp = y->get_pair();
				int max_sfx = x_tree.overlap( yp->get_seq() );
				if ( max_sfx >= threshold ) {
					xp->add_edge(yp, max_sfx);
				}
			}
		}
	}
}
//}}}

int main(int argc,char** argv) {

	if (argc < 6) {
		cout << "usage:\tnm_graph <node neighbor> <component neighbor> " 
			"<sfx threshold> <al file> <genome file>" << endl;
		cout << " node neighborhod: 600" << endl;
		cout << " component neighborhod: 1000" << endl;
		return 1;
	}

	//int node_neighbor = 600;
	//int component_neighbor = 1000;
	int node_neighbor = atoi(argv[1]);
	int component_neighbor = atoi(argv[2]);
	int sfx_T = atoi(argv[3]);

	seq_file seq(argv[5]);

	sv_graph *graph = new sv_graph();

	cerr << "Reading " << argv[4] << endl;
	ifstream file(argv[4]);
	if (file.is_open()) {
		graph->import_al(&file);
	} else {
		cerr << "Unable to open " << argv[4] << endl;
		return 1;
	}

	// get a vector of all nodes that are uniquely mapped and are paired with a
	// node that does not map
	vector<sv_graph_node*> *nm_pairs = NULL;
	//int num_nm_pairs = graph->get_nm_pairs(&nm_pairs);
	graph->get_nm_pairs(&nm_pairs);

	cerr << "Sorting U-NM pairs." << endl;
	sort(nm_pairs->begin(), nm_pairs->end(), 
			sv_graph_node::compare_sv_graph_nodes);

	/* 
	 * we want to maintain a fuzzy window size that will not cut off mid-island
	 * to do so we will set a max distance between a node and its neighbor, as
	 * long as there is a neighbor the window will be extended
	 */

	cerr << "Finding components: ";

	vector< u_nm_pair* > *components = new vector< u_nm_pair* >();

	vector<sv_graph_node*>::const_iterator i, j;
	//int node_neighbor = 600;
	i = nm_pairs->begin();

	while ( i != nm_pairs->end() ) {
		vector<sv_graph_node*> *nodes = new vector<sv_graph_node*>();

		//adjust window
		vector<sv_graph_node*>::const_iterator start, stop;
		start = i;
		while ( ( start != nm_pairs->begin()) &&
				( ( (*start)->get_offset() -
				    (*(start -1))->get_offset() ) < node_neighbor ) )
			--start;

		stop = i;
		while ( ( (stop + 1) != nm_pairs->end()) &&
				(  ((*(stop + 1))->get_offset() -
				   (*stop)->get_offset()) < node_neighbor ) )
			++stop;

		for (j = start; j != stop; j++)
			nodes->push_back(*j);

		/*
		 * At this point we have a ton of nodes that could be part of any
		 * number of circles, to dividie these into different individual
		 * circles we first connect nodes by their overlap, then for each node,
		 * we find all of the nodes that have a path to that node and place all
		 * of those nodes in a component.  The problem is that connencting all
		 * the nodes is an N^2 opperation, and we are adding about twice as
		 * many edges as we need to.
		 */

		//if (nodes->size() > 200) {
			vector<u_nm_pair*> *scs;
			sfx_join_sv_graph_nodes( nodes, sfx_T, &scs);
			components->insert( components->end(), scs->begin(), scs->end());
			delete scs;
		//}
		delete nodes;

		i = stop + 1;
	}

	cerr << components->size() << endl;

	sort(components->begin(),  components->end(), compare_u_nm_pair);

	vector<u_nm_pair*>::const_iterator k, start, stop;
	start = components->begin();
	stop = components->begin();

	vector< pair<u_nm_pair*, u_nm_pair*> > *pairs =
			new vector< pair<u_nm_pair*, u_nm_pair*> >();

	cerr << "Finding paris: ";
	for (k = components->begin(); k != components->end(); k++) {
		sfx_tree k_tree( (*k)->second );

		//Adjust window
		while (	(start != components->end()) && 
				( (*k)->first.first - (*start)->first.second > 
				  component_neighbor) )
			start++;

		while (	(stop != components->end()) && 
				( (*stop)->first.first - (*k)->first.second <
				  component_neighbor) )
			stop++;

		vector< u_nm_pair* >::const_iterator l;
		for (l = start; l != stop; l++) {
			sfx_tree l_tree( rev_comp( (*l)->second ) );

			int max_sfx_f = k_tree.overlap( rev_comp( (*l)->second ) );
			int max_sfx_r = l_tree.overlap( (*k)->second );

			int max_sfx = -1;

			if (max_sfx_f > max_sfx_r)
				max_sfx = max_sfx_f;
			else
				max_sfx = max_sfx_r;

			double a = (double)max_sfx / 
					   (double)( (*k)->second.length() );
			double b = (double)max_sfx / 
					   (double)( (*l)->second.length() );


			if ( (a > 0.5) || (b > 0.5) ) {
				pairs->push_back( pair<u_nm_pair*, u_nm_pair*> ( (*k), (*l) ) );

			//  Print out the comparisons
			/*
				cout << 
					(*k)->first.first << "\t" <<
					(*k)->first.second << "\t" <<
					(*l)->first.first << "\t" <<
					(*l)->first.second << "\t" <<
					max_sfx_f << "\t" <<
					max_sfx_r << "\t" <<
					(*k)->second << "\t" <<
					rev_comp( (*l)->second ) << "\t" << 
					a << "\t" <<
					b << "\t" <<
					endl;
			*/
			}
		}
	}

	cerr << pairs->size() << endl;

	return 1;

	/* 
	 * for u_nm_pairs: 
	 * min:first.first 
	 * max:first.first 
	 * seq:second
	 */
	vector< pair<u_nm_pair*, u_nm_pair*> >::const_iterator l;
	for(l = pairs->begin(); l != pairs->end(); l++) {
		u_nm_pair *a, *b;
		a = (*l).first;
		b = (*l).second;

		vector<int> x;
		x.push_back(a->first.second);
		x.push_back(a->first.first);
		x.push_back(b->first.second);
		x.push_back(b->first.first);
		int min,max;
		min = *min_element(x.begin(), x.end());
		max = *max_element(x.begin(), x.end());
		int range=500;
		if ( ( (max+range) - (min-range) ) < 2000 )
			cout << (min-range) << "\t" << (max+range) << "\t" <<
					seq.get_seq(min-range, max+range) << "\t" <<
					a->second << "\t" <<
					b->second << endl;
	}

	return 0;
}
