#ifndef __NOVO_LINE_H__
#define __NOVO_LINE_H__

#include <string>
#include <iostream>

using namespace std;

class novoalign_line {
	public:
		novoalign_line( string line, char start, map<string, string> *m );
		novoalign_line(string line, char start);
		string get_read_header();
		char get_read_type();
		string get_read_sequence();
		string get_base_qualities();
		char get_status();

		/*
		 * Alignment score is the log probability that the mapped sequence is
		 * the same as fragment read by the sequencer.  Alignment/Mapping
		 * quality is the log probability that this is the correct alignment
		 * location given that the DNA fragment came from the reference genome.
		 * It is a Bayesian posterior probability that is calculated from all
		 * the alignments found for the read.  Therefore lower scores are
		 * always better as score is -10log(P) where P is probability of the
		 * alignment. P=1.0 gives a score of zero.
		 */

		int get_alignment_score();
		int get_alignment_quality();
		string get_aligned_sequence();
		int get_aligned_offset();
		char get_strand();
		friend ostream& operator<<(ostream&, const novoalign_line&);

	private:
		string read_header;
		char read_type;
		string read_sequence;
		string base_qualities;
		char status;
		int alignment_score;
		int alignment_quality;
		string aligned_sequence;
		int aligned_offset;
		char strand;
};

//int parse_novoalign_file(FILE *file, char start, novoalign_line **n);
//int parse_novoalign_line(char *line, char start, novoalign_line **n);
//void free_novoalign_line(struct novoalign_line *n);
#endif
