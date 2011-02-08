CC=g++
AR=ar
CFLAGS= -Wall -Wno-deprecated -O3 
#CFLAGS= -Wall -Wno-deprecated -g
LDFLAGS= -lgsl -lgslcblas -lm 

EXECS=sfxtree_test parse_nodes nm_distro novoalign_al sfx_graph nm_distro \
	  nm_max_scs nm_seqs nm_graph ablink pair_distro seq_file_test \
	  sfx_test validate nm_group nm_oneside

SEQ_FILE_LIB=seq_file/seq_file.a
SEQ_FILE_OBJS=seq_file/seq_file.o

SV_GRAPH_LIB=sv_graph/libsv_graph.a
SV_GRAPH_OBJS=sv_graph/sv_graph.o sv_graph/sv_graph_node.o sv_graph/sv_edge.o \
	  sv_graph/sv_edge.o sv_graph/sv_graph_component.o

SFXTREE_LIB=sfxtree/libsfxtree.a
SFXTREE_OBJS=sfxtree/sfxtree.o sfxtree/sfxtree_node.o

SFX_LIB=sfx/libsfx.a
SFX_OBJS=sfx/sfx.o sfx/sfx_edge.o sfx/sfx_node.o sfx/sfx_tree.o

NOVOALIGN_LINE_LIB=novoalign_line/libnovoalign_line.a
NOVOALIGN_LINE_OBJS=novoalign_line/novoalign_line.o

OBJS=$(SEQ_FILE_OBJS) $(SV_GRAPH_OBJS) $(SFXTREE_OBJS) $(NOVOALIGN_LINE_OBJS) \
	 $(SFX_OBJS)
LIBS=$(SEQ_FILE_LIB) $(SV_GRAPH_LIB) $(SFXTREE_LIB) $(NOVOALIGN_LINE_LIB) \
	 $(SFX_LIB)

all: $(EXECS)

validate: validate.o $(SEQ_FILE_LIB) $(SV_GRAPH_LIB) $(SFX_LIB)
	$(CC) -o $@  $< $(SEQ_FILE_LIB) $(SV_GRAPH_LIB) $(SFX_LIB)

seq_file_test: seq_file_test.o $(SEQ_FILE_LIB)
	$(CC) -o $@  $< $(SEQ_FILE_LIB)

sfx_test: sfx_test.o $(SFX_LIB)
	$(CC) -o $@  $< $(SFX_LIB)

sfxtree_test: sfxtree_test.o $(SFXTREE_LIB)
	$(CC) -o $@  $< $(SFXTREE_LIB)

sfx_graph: sfx_graph.o $(SFXTREE_LIB) $(SV_GRAPH_LIB)
	$(CC) -o $@  $< $(SFXTREE_LIB) $(SV_GRAPH_LIB)

parse_nodes: parse_nodes.o $(SV_GRAPH_LIB)
	$(CC) $(LDFLAGS) -o $@  $< $(SV_GRAPH_LIB)

ablink: ablink.o $(SV_GRAPH_LIB) $(SFXTREE_LIB)
	$(CC) $(LDFLAGS) -o $@  $< $(SV_GRAPH_LIB) $(SFXTREE_LIB)

pair_distro: pair_distro.o $(SV_GRAPH_LIB) $(SFXTREE_LIB)
	$(CC) $(LDFLAGS) -o $@  $< $(SV_GRAPH_LIB) $(SFXTREE_LIB)

nm_graph: nm_graph.o $(SV_GRAPH_LIB) $(SFX_LIB) $(SEQ_FILE_LIB)
	$(CC) $(LDFLAGS) -o $@  $< $(SV_GRAPH_LIB) $(SFX_LIB) $(SEQ_FILE_LIB)

nm_oneside: nm_oneside.o $(SV_GRAPH_LIB) $(SFX_LIB) $(SEQ_FILE_LIB)
	$(CC) $(LDFLAGS) -o $@  $< $(SV_GRAPH_LIB) $(SFX_LIB) $(SEQ_FILE_LIB)

nm_group: nm_group.o $(SV_GRAPH_LIB) $(SFX_LIB) $(SEQ_FILE_LIB)
	$(CC) $(LDFLAGS) -o $@  $< $(SV_GRAPH_LIB) $(SFX_LIB) $(SEQ_FILE_LIB)

nm_seqs: nm_seqs.o $(SV_GRAPH_LIB)
	$(CC) $(LDFLAGS) -o $@  $< $(SV_GRAPH_LIB)

nm_distro: nm_distro.o $(SV_GRAPH_LIB)
	$(CC) $(LDFLAGS) -o $@  $< $(SV_GRAPH_LIB)

nm_max_scs: nm_max_scs.o $(SV_GRAPH_LIB)
	$(CC) $(LDFLAGS) -o $@  $< $(SV_GRAPH_LIB)

novoalign_al: novoalign_al.o $(NOVOALIGN_LINE_LIB)
	$(CC) -o $@  $< $(NOVOALIGN_LINE_LIB)

$(SFX_LIB): $(SFX_OBJS)
	$(AR) cq $@ $(SFX_OBJS)

$(SFXTREE_LIB): $(SFXTREE_OBJS)
	$(AR) cq $@ $(SFXTREE_OBJS)

$(SV_GRAPH_LIB): $(SV_GRAPH_OBJS)
	$(AR) cq $@ $(SV_GRAPH_OBJS)

$(SEQ_FILE_LIB): $(SEQ_FILE_OBJS)
	$(AR) cq $@ $(SEQ_FILE_OBJS)

$(NOVOALIGN_LINE_LIB): $(NOVOALIGN_LINE_OBJS)
	$(AR) cq $@ $(NOVOALIGN_LINE_OBJS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(EXECS) *.a *.o $(LIBS) $(OBJS)
