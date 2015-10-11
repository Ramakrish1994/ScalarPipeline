#include "simulator.h"

int main(){

    Simulator* simulator = new Simulator("input.txt", false,true);
	simulator->simulate();
	delete simulator;
	return 1;
}

