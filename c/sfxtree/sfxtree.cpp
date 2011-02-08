#include <algorithm>
#include <iostream>
#include <string>
#include <list>
#include <map>

using namespace std;

#include "sfxtree.h"
#include "sfxtree_node.h"

sfxtree::sfxtree(string t) {
	target = t;
	target += ";";

	int i=0, j=0, p_depth=0;
	sfxtree_node *p, *need_sfx=NULL;
	p = &root;

	int length = target.size();
	while( j < length ) {
		add(&i, &j, p, &p_depth, need_sfx);
		p_depth = max(0, p_depth);
		if (need_sfx == NULL) { need_sfx = &root;  }
		if (i > j) { j++; } 
	}
}

void sfxtree::add(int *i, int *j, sfxtree_node *p, int *p_depth, 
		sfxtree_node *need_sfx) {
	int s_left = (*j + 1 - *i) - *p_depth;
	char s_letter = target[*j];


	int p_len;
	sfxtree_node *r, *q;
	r = NULL;
	q = NULL;
	while ( (p != NULL) &&
			(s_left > (p_len = p->get_length())) ) {
		*p_depth = *p_depth + p_len;
		s_left = s_left - p_len;
		r = q;
		q = p;
		p = p->get_child( target[*i + *p_depth] );
	}

	int q_depth = *p_depth - q->get_length();

	if (p == NULL) { // substring is abset; we fell off the tree
		q->add_child(s_letter, *j, target.length() - *j, *i);
		if (need_sfx != NULL)
			need_sfx->set_sfx_link(q);
		
		if (q->get_sfx_link() != NULL) {
			*i = *i + 1;
			p = q->get_sfx_link();
			*p_depth = q_depth - 1;
			need_sfx = NULL;
		} else {
			*i = *i + 1;
			if (r != NULL) {
				p = r->get_sfx_link();
				*p_depth = q_depth - r->get_length() - 1;
			} else {
				p = NULL;
				*p_depth = q_depth - 1;
			}
			need_sfx = q;
		}	
	} else { // we're still on the tree, but the last letter may mismatch

		//q->add_child(s_letter, *j, target.length() - *j, *i);

		int p_off = p->get_offset();
		int p_len = p->get_length();
		char p_letter = target[p_off + s_left - 1];

		if (s_letter == p_letter) { // suffix is already there
			if (need_sfx != NULL)
				need_sfx->set_sfx_link(q);
			*j = *j + 1;
			p = q;
			*p_depth = q_depth;
			need_sfx = NULL;
		} else {
			sfxtree_node *n = new sfxtree_node(*j, target.length() - *j, *i);

			p->set_offset(p_off + s_left - 1);
			p->set_length(p_len - s_left + 1);

			sfxtree_node *m = new sfxtree_node(p_off, s_left - 1, -1);			

			m->add_child(p_letter, p);
			m->add_child(s_letter, n);

			q->add_child(target[*i + *p_depth], m);

			if (need_sfx != NULL)
				need_sfx->set_sfx_link(m);
			*i = *i + 1;
			p = q->get_sfx_link();
			*p_depth = q_depth - 1;
			need_sfx = m;
		}
	}
}


int sfxtree::max_suffix(string query) {
	query += ";";
	
	sfxtree_node *p = &root;
	sfxtree_node *q = &root;
	int q_len = query.length();
	int pat_len;
	string p_str;

	while (!query.empty()) {
		q = p;
		p = p->get_child(query[0]);
		if (p == NULL)
			break;
		pat_len = min(p->get_length(), q_len);
		p_str = target.substr(p->get_offset(), pat_len);

		if ( query.find(p_str) == 0 )
			query.erase(0, pat_len);
		else
			break;
		q_len = q_len - pat_len;
	}

	if (p == NULL) {
		/* 
		 * at this point we either went off the end of the tree, the last node
		 * did not have a matching child, in which case need to see if the
		 * parent has a child with the end of string character, if so then
		 * return that nodes startsAt, otherwise return 0.
		 */
		p = q->get_child(';');

		if (p != NULL) 
			return target.length() - (p->get_starts_at() + 1);
		else
			return 0;
	} else {
		/*
		 * at this point we have either gone completely through the query or
		 * the content of the current node did not match the remaining query
		 * string
		 */
		if (q_len == 0)
			return target.length() - (p->get_starts_at() + 1);
		else {
			/*
			 * when the current node doesnt match the rest of the query
			 * we could have a partial match, one where the query ends in the
			 * middle of the one, and one where it doesn't
			 */
			if ( (p_str[p_str.length() - 1] == ';' ) &&
				( query.find( p_str.substr(0, p_str.length() - 1) ) == 0 ) ) {
				return target.length() - (p->get_starts_at() + 1);
			} else {
				p = q->get_child(';');
				if (p != NULL) 
					return target.length() - (p->get_starts_at() + 1);
				else
					return 0;
			}
		}
	}	
}

void sfxtree::print_tree() {
	list<sfxtree_node*> q;

	q.push_back(&root);

	while (!q.empty()) {
		sfxtree_node *c = q.front();
		map<char, sfxtree_node*> children = c->get_children();
		map<char, sfxtree_node*>::iterator it;

		for (it	= children.begin(); it != children.end(); it++) {
			cout << target.substr(c->get_offset(), c->get_length()) << "\t" <<
					c << "\t" << 
					target.substr(it->second->get_offset(),
							it->second->get_length()) << "\t" <<
							it->second->get_starts_at()
					<< "\t" << it->second << endl;
			q.push_back(it->second);
		}
		q.pop_front();
	}
}

int sfxtree::max_suffix_2(string query) {
	query += ";";
	
	sfxtree_node *p = &root;
	sfxtree_node *q = &root;
	int q_len = query.length();
	int q_len_o = query.length();
	int pat_len;
	string p_str;

	while (!query.empty()) {
		cout << "+" << p->get_length() << endl;
		q = p;
		p = p->get_child(query[0]);
		if (p == NULL)
			break;
		pat_len = min(p->get_length(), q_len);
		p_str = target.substr(p->get_offset(), pat_len);

		if ( query.find(p_str) == 0 )
			query.erase(0, pat_len);
		else {
			int i;
			for (i = 0; i < p_str.length(); i++) {
				if (query[i] == p_str[i])
					q_len--;
				else
					break;
			}
			break;
		}
		q_len = q_len - pat_len;
	}

	return q_len_o - q_len;
}
