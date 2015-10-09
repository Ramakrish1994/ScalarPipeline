#include "simulator.h"

int main(){

    Simulator* simulator = new Simulator("input.txt");
	simulator->simulate();
	delete simulator;
	return 1;
}

