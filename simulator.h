#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "branch_predictor.h"

#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#define NUM_REGISTERS 16
#define DEPTH_PIPELINE 6

using namespace std;

struct pipeline_instr {
	long long int pc;
	string IR;
	string display_IR;
	int opcode;
	bool immediate;
	int op1, op2, op3;
	int A, B, imm_field;
	int cond;
	int alu_output;
	int load_md;
	//copy constructor
};

class Simulator{
	long long int m_clk;
	bool control_flag;

	map<int, int> d_cache;
	map<long long int, string> i_cache;

	vector<long long int> register_status;

	long long int pc;

	int register_file[NUM_REGISTERS];

	struct pipeline_instr ins_pipeline[DEPTH_PIPELINE];
	
	int ins_index[DEPTH_PIPELINE];
	
	void next_clock_cycle(){
		for(int i=0; i<DEPTH_PIPELINE; i++)
			ins_index[i] = (ins_index[i]+1)%6;
		m_clk ++;
	}

	bool prev_ins_decoded_is_branch;

	pipeline_instr temp_fetch, temp_decode;
	bool raw_flag;
	bool prev_raw_flag;
	bool unset_raw_flag_cycle;
	bool wait_on_reg[NUM_REGISTERS];
	string input_code;
	ofstream out;

	void load_i_cache();
	void print_i_cache();
	void print_d_cache();
	void print_reg_file();

	bool branch_pred_enabled;
	bool flush_pipeline;
	map<long long int, long long int> btb; 
	map<long long int, BranchPredictor> bp;
	void flush();
	
public:
	Simulator(string input_file, bool b_enable);
	int fetch(int ins_index);
	int decode(int ins_index);
	int register_read(int ins_index);
	int execute(int ins_index);
	int mem_branch_cycle(int ins_index);
	int write_back(int ins_index);
	int simulate();
};

int gen_int_from_string(string s);
int get_twos_complement(string s);

#endif
