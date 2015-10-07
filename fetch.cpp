#include <pipeline.h>

class fetch{
		int ins_index;
	public:
		decode(){
			ins_index = 6;
		}
		int process_next_instruction(){

			ins_index = next_ins_index(int ins_index);
			return 1;
		}
}