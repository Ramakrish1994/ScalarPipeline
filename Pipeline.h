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
