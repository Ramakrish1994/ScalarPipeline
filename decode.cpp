#include <pipeline.h>

class decode{
		int ins_index;
	public:
		decode(){
			ins_index = 5;
		}
		int process_next_instruction(){

			ins_index = next_ins_index(int ins_index);
			return 1;
		}
}