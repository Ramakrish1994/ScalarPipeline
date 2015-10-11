#include "simulator.h"

int main(int argc, char* argv[]){


    Simulator* simulator = new Simulator(argv[1], false,false);
	simulator->simulate();
	delete simulator;
	return 1;
}

