#include <vector>
#include <map>
#include <string>
using namespace std;
#define NUM_REGISTERS 16


struct pipeline_instr {
	string IR;
	int opcode;
	bool immediate;
	int op1, op2, op3;
	int jump_offset;
	int alu_output;
	int load_md;
};

class simulator{
	long long int clk;
	bool control_flag;

	map<long long int, int> d_cache;
	map<long long int, string> i_cache;

	vector<bool> register_status;

	long long int pc;

	long long int register_file[NUM_REGISTERS];

	struct pipeline_instr ins_pipeline[6];
	
	int ins_index[6];
	
	int next_clock_cycle(){
		for(int i=0; i<6; i++)
			ins_index[i] = (ins_index[i]+1)%7;
		clk ++;
	}

	bool prev_ins_decoded_is_branch;

	
public:
	simulator(){
		ins_index[0] = 5;//for fetch
		ins_index[1] = 4;
		ins_index[2] = 3;
		ins_index[3] = 2;
		ins_index[4] = 1;
		ins_index[5] = 0;

		for(int i=0;i<6;i++){
			ins_pipeline[i].opcode = 7;
			ins_pipeline[i].IR = "1110000000000000"; //assuming 16 bit instructions
		}

		//decode stage
		prev_ins_decoded_is_branch = false;

		clk = 0;

	}

	int decode(int ins_index){

		if(prev_ins_decoded_is_branch){
				prev_ins_decoded_is_branch = false;
				ins_pipeline[ins_index].opcode = 7;
			}

		// need to fill in

		return 1;
	}



	int simulate(){

		while(1){
			is_last_ins = fetch(ins_index[0]);
			decode(ins_index[1]);
			// rest of calls

			next_clock_cycle();//increments clk and the index

			//break based on is_last_ins

		}

	}


};

int main(){
	return 1;
}