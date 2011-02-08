#include <string>
#include <iostream>
#include <string.h>
#include "sfx.h"
#include "sfx_edge.h"
#include "sfx_tree.h"
#include "sfx_node.h"

int sfx_tree::MAX_LENGTH = 1000;
int sfx_tree::HASH_TABLE_SIZE = 2179;  
char sfx_tree::END_CHAR = '#';  

sfx_tree::
sfx_tree(string t) {

	t.append(1, sfx_tree::END_CHAR);

	string s = t;
	
	edges = new sfx_edge[ HASH_TABLE_SIZE ];
	nodes = new sfx_node[ MAX_LENGTH * 2]; 

	N = t.length() - 1;
	T = new char[t.length() + 1];
	strcpy(T, t.c_str());

	count = 0;

	sfx active( 0, 0, -1 );  // The initial active prefix

	for (int i = 0; i <= N; i++)
		add_prefix(active, i);
}

sfx_tree::
~sfx_tree() {
	delete[] edges;
	delete[] nodes;
}

void
sfx_tree::
add_prefix(sfx &active, int last_char_index)
{
    int parent_node;
    int last_parent_node = -1;

    for ( ; ; ) {
        sfx_edge edge;
        parent_node = active.origin_node;
//
// Step 1 is to try and find a matching edge for the given node.
// If a matching edge exists, we are done adding edges, so we break
// out of this big loop.
//
        if ( active.expl() ) {
            edge = find_edge( active.origin_node, T[ last_char_index ] );
            if ( edge.start_node != -1 )
                break;
        } else { //implicit node, a little more complicated
            edge = find_edge( active.origin_node, T[ active.first_char_index ] );
            int span = active.last_char_index - active.first_char_index;
            if ( T[ edge.first_char_index + span + 1 ] == T[ last_char_index ] )
                break;
            //parent_node = edge.split_edge( active );
            parent_node = split_edge(active, edge);
        }
//
// We didn't find a matching edge, so we create a new one, add
// it to the tree at the parent node position, and insert it
// into the hash table.  When we create a new node, it also
// means we need to create a suffix link to the new node from
// the last node we visited.
//
        sfx_edge *new_edge = new sfx_edge( last_char_index, N, parent_node, 
				count ++);
        insert_edge(*new_edge);
        if ( last_parent_node > 0 )
            nodes[ last_parent_node ].suffix_node = parent_node;
        last_parent_node = parent_node;
//
// This final step is where we move to the next smaller suffix
//
        if ( active.origin_node == 0 )
            active.first_char_index++;
        else
            active.origin_node = nodes[ active.origin_node ].suffix_node;
        canonize(active);

		delete new_edge;
    }
    if ( last_parent_node > 0 )
        nodes[ last_parent_node ].suffix_node = parent_node;
    active.last_char_index++;  //Now the endpoint is the next active point
    canonize(active);
}

sfx_edge
sfx_tree::
find_edge(int node, int c)
{  
	int i = hash(node, c);
	for ( ; ; ) {
		if ( edges[i].start_node == node )
			if ( c == T[ edges[ i ].first_char_index ] )
				return edges[ i ];
			if ( edges[ i ].start_node == -1 )
		return edges[ i ];
		//i = ++i % HASH_TABLE_SIZE;
		i = (i + 1) % HASH_TABLE_SIZE;
	}
}

int
sfx_tree::
hash(int node, int c) {
	return ( ( node << 8 ) + c ) % HASH_TABLE_SIZE;
}

int
sfx_tree::
split_edge(sfx &s, sfx_edge &e)
{
	remove_edge(e);
	sfx_edge *new_edge = new sfx_edge( e.first_char_index,
			e.first_char_index + s.last_char_index - s.first_char_index,
			s.origin_node,
			count++);
	//new_edge->insert();
	insert_edge(*new_edge);
	nodes[ new_edge->end_node ].suffix_node = s.origin_node;
	e.first_char_index += s.last_char_index - s.first_char_index + 1;
	e.start_node = new_edge->end_node;
	//Insert();
	insert_edge(e);

	int r = new_edge->end_node;
	delete new_edge;
	//return new_edge->end_node;
	return r;
}

void 
sfx_tree::
remove_edge(sfx_edge &e)
{   
	int i = hash( e.start_node, T[ e.first_char_index ] );
	while ( edges[ i ].start_node != e.start_node || 
			edges[ i ].first_char_index != e.first_char_index )
		i = (i + 1) % HASH_TABLE_SIZE;
		//i = ++i % HASH_TABLE_SIZE;

	for ( ; ; ) {
		edges[ i ].start_node = -1;
		int j = i;
		for ( ; ; ) {
			i = (i + 1) % HASH_TABLE_SIZE;
			//i = ++i % HASH_TABLE_SIZE;
			if ( edges[ i ].start_node == -1 )
				return;
			int r = hash( edges[ i ].start_node, 
					T[ edges[ i ].first_char_index ] );
			if ( (i >= r) && (r > j) )
				continue;
			if ( (r > j) && (j > i ) )
				continue;
			if ( (j > i) && (i >= r) )
				continue;
			break;
		}

		edges[ j ] = edges[ i ];
	}
}

void 
sfx_tree::
insert_edge(sfx_edge &e)
{
	int i = hash( e.start_node, T[ e.first_char_index ] );
	while ( edges[ i ].start_node != -1 )
		i = (i + 1) % HASH_TABLE_SIZE;
		//i = ++i % HASH_TABLE_SIZE;
	edges[ i ] = e;
}

void 
sfx_tree::
canonize(sfx &s)
{   
	if ( !s.expl() ) {
		sfx_edge edge = find_edge( s.origin_node, T[ s.first_char_index ] );
		int edge_span = edge.last_char_index - edge.first_char_index;
		while ( edge_span <= ( s.last_char_index - s.first_char_index ) ) {
			s.first_char_index = s.first_char_index + edge_span + 1;
			s.origin_node = edge.end_node;
			if ( s.first_char_index <= s.last_char_index ) {
				edge = find_edge( edge.end_node, T[ s.first_char_index ] );
				edge_span = edge.last_char_index - edge.first_char_index;
			}
		}
	}
}


void
sfx_tree::
search (char *query, int &start_index, int &end_index)
{
	int start_node = 0;
	int qp = 0; //query position
	start_index = -1;
	end_index = -1;

	bool stop = false;
	while (!stop){
		sfx_edge edge = find_edge(start_node, query[qp]);
		if ( edge.start_node == -1) {
			stop=true;
			break;
		}
		if (start_node ==0)
			start_index = edge.first_char_index;
		//print_edge(edge);
		for (int i = edge.first_char_index; i <= edge.last_char_index; i++){
			if( qp>= strlen(query) ) {
                //cout<<"whole query matched"<<endl;
				stop=true;
				break;
			} else if (query[qp] == T[i]){
				//cout<<query[qp]<<" ";
				qp++;
				end_index = i;
			} else {
				//cout<<"partially matched"<<endl;
				stop=true;
				break;
			}
		}
		if (!stop) { //proceed with next node
			start_node = edge.end_node;
			if(start_node==-1) stop=true;
				//cout<<"next node    "<<start_node;
		}   
	}
}

int
sfx_tree::
overlap (string q)
{
	char t[q.length()];
	strcpy(t, q.c_str());
	return overlap(t);
}


int
sfx_tree::
overlap (char *q)
{

	char query[strlen(q) + 1];
	strcpy(query, q);
	query[strlen(q)] = sfx_tree::END_CHAR;
	query[strlen(q) + 1] = '\0';

	int start_node = 0;
	int qp = 0; //query position
	//int start_index = -1;
	//int end_index = -1;

	while ( qp < strlen(q) ) {
		sfx_edge edge = find_edge(start_node, query[qp]);

		if ( edge.start_node == -1 ) {
			edge = find_edge(start_node, sfx_tree::END_CHAR);
			if ( edge.start_node == -1 ) {
				return 0;
			} else {
				return qp;
			}
		}

		for (int i = edge.first_char_index; i <= edge.last_char_index; i++){
			if (qp == strlen(q) )
				return qp;

			if (query[qp] == T[i]){
				qp++;
			} else {
				if (T[i] == sfx_tree::END_CHAR) {
					return qp;
				}
				return 0;
			}
		}

		start_node = edge.end_node;
	}

	return qp - 1;
}

