#include <vector>
#include <map>
#include <string>
#include <iostream>
using namespace std;
#define NUM_REGISTERS 16
#define DEPTH_PIPELINE 6


struct pipeline_instr {
	long long int pc;
	string IR;
	int opcode;
	bool immediate;
	int op1, op2, op3;
	int A, B, imm_field;
	int cond;
	int alu_output;
	int load_md;
	//copy constructor
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

int get_twos_complement(string s) {
	int num = 0;
	if (s[0] == '0') {
		return get_int_from_string(s);
	}
	else {
		// if 2'complement of a negative no y is x then ~x + 1 = -y
		for(int i = 0; i < s.size(); ++i) {
			s[i] = 1 - s[i];
		}
		num = get_int_from_string(s);
		num++;
		return (-num);
	}
}

class simulator{
	long long int clk;
	bool control_flag;

	map<long long int, int> d_cache;
	map<long long int, string> i_cache;

	vector<long long int> register_status;

	long long int pc;

	long long int register_file[NUM_REGISTERS];

	struct pipeline_instr ins_pipeline[DEPTH_PIPELINE];
	
	int ins_index[DEPTH_PIPELINE];
	
	int next_clock_cycle(){
		for(int i=0; i<DEPTH_PIPELINE; i++)
			ins_index[i] = (ins_index[i]+1)%7;
		clk ++;
	}

	bool prev_ins_decoded_is_branch;

	pipeline_instr temp_fetch, temp_decode;
	bool raw_flag;
	bool prev_raw_flag;
	bool wait_on_reg[NUM_REGISTERS];
	
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
			ins_pipeline[i].IR = "1111000000000000"; //assuming 16 bit instructions
		}

		temp_decode.opcode = 7;
		temp_decode.IR = "1111000000000000";

		temp_fetch.opcode = 7;
		temp_fetch.IR = "1111000000000000";
		//decode stage
		prev_ins_decoded_is_branch = false;
		control_flag = false;
		clk = 0;

		pc = 0;
		for(int i = 0; i < NUM_REGISTERS; ++i) {
			register_status.push_back(-1);
			register_file[i] = 0;
			wait_on_reg[i] = false;
		}

		register_file[0] = 0;

		raw_flag = false;

		prev_raw_flag = false;


	}

	int fetch(int ins_index) {
		pipeline_instr p;
		if (control_flag || raw_flag) {
			p.IR = "1111000000000000";
		}
		else {
			p.IR = i_cache[pc];
			p.pc = pc;
			pc += 2;
		}
		ins_pipeline[ins_index] = p;
	}


	int decode(int ins_index){

		bool is_raw = false;

		if (prev_raw_flag){
			prev_raw_flag = false;
			temp_fetch = ins_pipeline[ins_index];
			ins_pipeline[ins_index].opcode = 7;
		}

		if(raw_flag)
			ins_pipeline[ins_index].opcode = 7;

		if(prev_ins_decoded_is_branch){
				prev_ins_decoded_is_branch = false;
				ins_pipeline[ins_index].opcode = 7;
				pc -= 2;
			}

		string IR = ins_pipeline[ins_index].IR;
		ins_pipeline[ins_index].opcode = get_int_from_string(IR.substr(0,3));
		ins_pipeline[ins_index].immediate = (get_int_from_string(IR.substr(3,1)) >= 1)?true:false;

		if(ins_pipeline[ins_index].opcode == 7 && ins_pipeline[ins_index].immediate == 0)
			ins_pipeline[ins_index].opcode = 8;
		
		if (ins_pipeline[ins_index].opcode == 5 ){ // jump instruction
			ins_pipeline[ins_index].op1 = get_twos_complement(IR.substr(4,8));
			prev_ins_decoded_is_branch = true;
			control_flag = true;

		}
		else if (ins_pipeline[ins_index].opcode == 6){ // BEQZ
			ins_pipeline[ins_index].op1 = get_int_from_string(IR.substr(4,4));
			ins_pipeline[ins_index].op2 = get_twos_complement(IR.substr(8,8));
			prev_ins_decoded_is_branch = true;
			control_flag = true;

			if (register_status[ins_pipeline[ins_index].op1] != -1){
				is_raw = true;
				wait_on_reg[ins_pipeline[ins_index].op1] = true;
			}

		}
		else{ // add, sub, mul, ld,st
			ins_pipeline[ins_index].op1 = get_int_from_string(IR.substr(4,4));
			ins_pipeline[ins_index].op2 = get_int_from_string(IR.substr(8,4));
			if (ins_pipeline[ins_index].immediate) {
				ins_pipeline[ins_index].op3 = get_twos_complement(IR.substr(12,4));
			}
			else {
				ins_pipeline[ins_index].op3 = get_int_from_string(IR.substr(12,4));
			}

			 
			if( (ins_pipeline[ins_index].opcode <= 2 && ins_pipeline[ins_index].immediate) || ins_pipeline[ins_index].opcode == 3 )
				if (register_status[ins_pipeline[ins_index].op2] != -1){
					is_raw = true;
					wait_on_reg[ins_pipeline[ins_index].op2] = true;
				}

			if( (ins_pipeline[ins_index].opcode <=2 && !ins_pipeline[ins_index].immediate) )
				if (register_status[ins_pipeline[ins_index].op2] != -1){
					wait_on_reg[ins_pipeline[ins_index].op2] = true;
					is_raw = true;
				} 
				if (register_status[ins_pipeline[ins_index].op3] != -1){
					wait_on_reg[ins_pipeline[ins_index].op3] = true;
					is_raw = true;
				}
					

			if( (ins_pipeline[ins_index].opcode == 4 ) )
				if (register_status[ins_pipeline[ins_index].op1] != -1){
					wait_on_reg[ins_pipeline[ins_index].op1] = true;
					is_raw = true;
				}
				if (register_status[ins_pipeline[ins_index].op2] != -1){
					wait_on_reg[ins_pipeline[ins_index].op2] = true;
					is_raw = true;
				}
					

		}

		if (is_raw){
			raw_flag = true;
			prev_raw_flag = true;
			temp_decode = ins_pipeline[ins_index];
		}

		else{

			if(ins_pipeline[ins_index].opcode <= 3)
				register_status[ins_pipeline[ins_index].op1] = pc;
		}

		

		return 1;
	}


	int register_read(int ins_index) {
		if (ins_pipeline[ins_index].opcode <= 2) {
			ins_pipeline[ins_index].A = register_file[ins_pipeline[ins_index].op2];
			if(ins_pipeline[ins_index].immediate) {
				ins_pipeline[ins_index].imm_field = ins_pipeline[ins_index].op3;
			}
			else {
				ins_pipeline[ins_index].B = register_file[ins_pipeline[ins_index].op3];
			}
		}
		else if (ins_pipeline[ins_index].opcode == 3) {
			ins_pipeline[ins_index].A = register_file[ins_pipeline[ins_index].op2];
		}
		else if (ins_pipeline[ins_index].opcode == 4) {
			ins_pipeline[ins_index].A = register_file[ins_pipeline[ins_index].op1];
			ins_pipeline[ins_index].B = register_file[ins_pipeline[ins_index].op2];
		}
		else if(ins_pipeline[ins_index].opcode == 5) {
			ins_pipeline[ins_index].imm_field = ins_pipeline[ins_index].op1;
		}
		else if(ins_pipeline[ins_index].opcode == 6) {
			ins_pipeline[ins_index].A = register_file[ins_pipeline[ins_index].op1];
			ins_pipeline[ins_index].imm_field = ins_pipeline[ins_index].op2;
		}
	}	

	int execute (int ins_index){
		long long int A = ins_pipeline[ins_index].A;
		long long int B = ins_pipeline[ins_index].B;
		long long int imm_field = ins_pipeline[ins_index].imm_field;

		switch(ins_pipeline[ins_index].opcode){

			case 0: if (ins_pipeline[ins_index].immediate) ins_pipeline[ins_index].alu_output = A + imm_field;
					else ins_pipeline[ins_index].alu_output = A + B; break;
			case 1: if (ins_pipeline[ins_index].immediate) ins_pipeline[ins_index].alu_output = A - imm_field;
					else ins_pipeline[ins_index].alu_output = A - B; break;
			case 2: if (ins_pipeline[ins_index].immediate) ins_pipeline[ins_index].alu_output = A * imm_field;
					else ins_pipeline[ins_index].alu_output = A * B; break;

			case 3: ins_pipeline[ins_index].alu_output = 0 + A; break;
			case 4: ins_pipeline[ins_index].alu_output = 0 + A; break;

			case 5: ins_pipeline[ins_index].alu_output = pc + (imm_field<<1); ins_pipeline[ins_index].cond = 1; break;
			case 6: ins_pipeline[ins_index].alu_output = pc + (imm_field<<1); ins_pipeline[ins_index].cond = (A == 0); break;
		}
		return 1;
	}

	int mem_branch_cycle (int ins_index) {
		if (ins_pipeline[ins_index].opcode == 3) {
			ins_pipeline[ins_index].load_md = d_cache[ins_pipeline[ins_index].alu_output];
		}
		else if (ins_pipeline[ins_index].opcode == 4) {
			d_cache[ins_pipeline[ins_index].alu_output] = ins_pipeline[ins_index].B;
		}
		else if (ins_pipeline[ins_index].opcode == 5) {
			pc = ins_pipeline[ins_index].alu_output;
			control_flag = false;
		}
		else if (ins_pipeline[ins_index].opcode == 6) {
			if (ins_pipeline[ins_index].cond) {
				pc = ins_pipeline[ins_index].alu_output;
				control_flag = false;
			}
		}
	}

	int write_back(int ins_index){
		if (ins_pipeline[ins_index].opcode <=2)
			register_file[ins_pipeline[ins_index].op1] = ins_pipeline[ins_index].alu_output;
		if (ins_pipeline[ins_index].opcode == 3)
			register_file[ins_pipeline[ins_index].op1] = ins_pipeline[ins_index].load_md;


		if (ins_pipeline[ins_index].opcode <= 3){
			if(register_status[ins_pipeline[ins_index].op1] == ins_pipeline[ins_index].pc)
				register_status[ins_pipeline[ins_index].op1] = -1;
		}

		if (raw_flag){
			bool raw_not_over = false;
			for(int i =0; i <NUM_REGISTERS; i++){
				if(wait_on_reg[i] == true && register_status[i] == -1)
					wait_on_reg[i] = false;
				else if (wait_on_reg[i] == true && register_status[i] != -1)
					raw_not_over = true;
				
			}

			if(!raw_not_over){
				raw_flag = false;
				if(temp_decode.opcode <=3)
					register_status[temp_decode.op1] = temp_decode.pc;
				
			}
		}
		if (ins_pipeline[ins_index].opcode == 8)
			return 0;
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
