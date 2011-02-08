date
mkdir EH
cd EH
echo EH

time ../novoalign_al /data/projects/Circle/Seq/05082010/EH_s_7_1_sequence.out,/data/projects/Circle/Seq/05082010/EH_s_7_2_sequence.out "@" /home/rl6sf/data/mouse.map
time ../nm_graph 10 2 20 17 /data/reference/genome/mouse/mm_ref_chr17.fa > nm_graph.EH.10.2.20-17
cat nm_graph.EH.10.2.20-17 | ../../perl/lcs.pl | sort -n | uniq > nm_graph.EH.10.2.20-17.bed

cd ..

mkdir EL
cd EL
echo EL

time ../novoalign_al /data/projects/Circle/Seq/05082010/EL_s_8_1_sequence.out,/data/projects/Circle/Seq/05082010/EL_s_8_2_sequence.out "@" /home/rl6sf/data/mouse.map
time ../nm_graph 10 2 20 17 /data/reference/genome/mouse/mm_ref_chr17.fa > nm_graph.EL.10.2.20-17
cat nm_graph.EL.10.2.20-17 | ../../perl/lcs.pl | sort -n | uniq > nm_graph.EL.10.2.20-17.bed

cd ..

mkdir G
cd G
echo G

time ../novoalign_al /data/projects/Circle/Seq/05082010/G_s_4_1_sequence.out,/data/projects/Circle/Seq/05082010/G_s_4_2_sequence.out "@" /home/rl6sf/data/mouse.map
time ../nm_graph 10 2 20 17 /data/reference/genome/mouse/mm_ref_chr17.fa > nm_graph.G.10.2.20-17
cat nm_graph.G.10.2.20-17 | ../../perl/lcs.pl | sort -n | uniq > nm_graph.G.10.2.20-17.bed

cd ..

cd AB
time ../nm_graph 10 2 20 17 /data/reference/genome/mouse/mm_ref_chr17.fa > nm_graph.AB.10.2.20-17
cat nm_graph.AB.10.2.20-17 | ../../perl/lcs.pl | sort -n | uniq > nm_graph.AB.10.2.20-17.bed

cd ..
