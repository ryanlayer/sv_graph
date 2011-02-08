#include "sv_graph_node.h"
#include "sv_edge.h"

sv_edge::
sv_edge(sv_graph_node *s, sv_graph_node *t, int w, int c) {
	source = s;
	target = t;
	weight = w;
	color = c;
}

sv_edge::
sv_edge(sv_graph_node *s, sv_graph_node *t) {
	source = s;
	target = t;
	weight = -1;
	color = -1;
}

sv_graph_node* 
sv_edge::
get_target()
{
	return target;
}

void
sv_edge::
set_target(sv_graph_node *t)
{
	target = t;
}

sv_graph_node*
sv_edge::
get_source()
{
	return source;
}

void
sv_edge::
set_source(sv_graph_node *s)
{
	source = s;
}

void
sv_edge::
set_weight(int w)
{
	weight = w;
}

int
sv_edge::
get_weight()
{
	return weight;
}

void
sv_edge::
set_color(int c)
{
	color = c;
}

int
sv_edge::
get_color()
{
	return color;
}

bool
sv_edge::
//operator<(const sv_edge &a, const sv_edge &b) const
operator<(sv_edge &other)
{
	//return a.get_weight() < b.get_weight();
	return weight < other.get_weight();
}
