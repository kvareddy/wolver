#include <iostream>
#include <stdio.h>
#include "wolmgr.h"
#include "wolexp.h"
#include "wolsmt2.h"
#include "wolexpfactory.h"

namespace wolver {

int wolver_main(int argc, char **argv)
{

//	std::cout << "Hello Word Solver" << std::endl;
	WolMgr *mgr = new WolMgr();
   WolExpFactory *factory = new WolExpFactory(mgr);
	FILE *input_file = fopen(argv[1], "r");

	if (!input_file) {
		std::cout << "error: input file does not exist" << std::endl;	
		exit(1);
	}
	WolSmt2Parser *parser = new WolSmt2Parser(mgr, factory, input_file);
   if (parser->parse())	
      parser->print();
   else 
      std::cout << "error while reading the smt file" <<endl;
	
	return 0;
}

}

int main(int argc, char **argv) {
	wolver::wolver_main(argc, argv);

	return 0;
}

