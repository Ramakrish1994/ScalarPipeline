#include<vector>
#include<map>
#include<string>
#define NUM_REGISTERS 16

extern bool control_flag;

extern unordered_map<long long int, int> i_cache, d_cache;

extern vector<bool> register_status;

extern long long int pc;

struct pipeline_instr {
	string IR;
	int opcode;
	bool immediate;
	int op1, op2, op3;
	int jump_offset;
	int alu_output;
	int load_md;
};

struct pipeline_instr ins_pipeline[7];
//initiate index of fetch to be 6, decode - 5 ,....
//start filling from 0 in the main function and run the system
int next_ins_index(int ind){
	return (ind+1)%7;
} 