#include <vector>
#include <map>
#include <string>
#include <iostream>
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


int get_int_from_string(string s){
	int num = 0;
	int len = s.length();
	int pow2 = 1;
	for(int i = len-1; i >= 0; i--){
		if(s[i] == '1')
			num += pow2;
		pow2 *= 2;

	}
	return num;
}

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
		control_flag = false;
		clk = 0;

	}

	int decode(int ins_index){

		if(prev_ins_decoded_is_branch){
				prev_ins_decoded_is_branch = false;
				ins_pipeline[ins_index].opcode = 7;
			}

		string IR = ins_pipeline[ins_index].IR;
		ins_pipeline[ins_index].opcode = get_int_from_string(IR.substr(0,3));
		ins_pipeline[ins_index].immediate = (get_int_from_string(IR.substr(3,1)) >= 1)?true:false;
		
		if (ins_pipeline[ins_index].opcode == 5 ){ // jump instruction
			ins_pipeline[ins_index].op1 = get_int_from_string(IR.substr(4,8));
		}
		else if (ins_pipeline[ins_index].opcode == 6){ // BEQZ
			ins_pipeline[ins_index].op1 = get_int_from_string(IR.substr(4,4));
			ins_pipeline[ins_index].op2 = get_int_from_string(IR.substr(8,8));
			prev_ins_decoded_is_branch = true;
			control_flag = true;
		}
		else{ // add, sub, mul, ld,st
			ins_pipeline[ins_index].op1 = get_int_from_string(IR.substr(4,4));
			ins_pipeline[ins_index].op2 = get_int_from_string(IR.substr(8,4));
			ins_pipeline[ins_index].op3 = get_int_from_string(IR.substr(12,4));
			prev_ins_decoded_is_branch = true;
			control_flag = true;

		}

		return 1;
	}



	int simulate(){

		while(1){
			// int is_last_ins = fetch(ins_index[0]);
			// decode(ins_index[1]);
			// // rest of calls

			// next_clock_cycle();//increments clk and the index

			// //break based on is_last_ins

		}

	}


};

int main(){

	
	return 1;
}