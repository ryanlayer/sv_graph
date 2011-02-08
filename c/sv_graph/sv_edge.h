class sv_graph_node;
#ifndef __SV_EDGE__
#define __SV_EDGE__

#include "sv_graph_node.h" 

class sv_edge {

public:
	sv_edge(sv_graph_node *s, sv_graph_node *t, int weight, int color);
	sv_edge(sv_graph_node *s, sv_graph_node *t);
	sv_graph_node* get_target();
	void set_target(sv_graph_node *t);
	sv_graph_node* get_source();
	void set_source(sv_graph_node *s);
	void set_weight(int w);
	int get_weight();
	void set_color(int c);
	int get_color();
	//bool operator<(const sv_edge &a, const sv_edge &b) const;
	inline bool operator<(sv_edge &other);

private:
	int weight;
	int color;
	sv_graph_node *source, *target;

};

#endif
