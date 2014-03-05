#include <iostream>
#include <stdio.h>
#include "wolmgr.h"
#include "wolexp.h"
#include "wolsmt2.h"

namespace wolver {

int wolver_main(int argc, char **argv)
{

	std::cout << "Hello Word Solver" << std::endl;
	WolMgr *mgr = new WolMgr();
	FILE *input_file = fopen(argv[1], "r");

	if (!input_file) {
		std::cout << "error: input file does not exist" << std::endl;	
		exit(1);
	}
	WolSmt2Parser *parser = new WolSmt2Parser(mgr, input_file);
	parser->parse();	
	
	return 0;
}

}

int main(int argc, char **argv) {
	wolver::wolver_main(argc, argv);

	return 0;
}

