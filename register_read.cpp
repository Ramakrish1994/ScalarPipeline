#include <pipeline.h>

class register_read{
		int ins_index;
	public:
		decode(){
			ins_index = 4;
		}
		int process_next_instruction(){

			ins_index = next_ins_index(int ins_index);
			return 1;
		}
}