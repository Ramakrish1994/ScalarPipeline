#include "simulator.h"

int main(){

    Simulator* simulator = new Simulator("input.txt", true);
	simulator->simulate();
	delete simulator;
	return 1;
}

