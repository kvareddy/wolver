#include <iostream>
#include <stdio.h>
#include "wolmgr.h"
#include "wolexp.h"
#include "wolsmt2.h"
#include "wolexpfactory.h"
#include "wolevalfactory.h"
#include "wolvaluefactory.h"
#include "wolvaluefactoryimpl.h"
#include "wolevalfactoryimpl.h"

namespace wolver {

int wolver_main(int argc, char **argv)
{

//	std::cout << "Hello Word Solver" << std::endl;
	FILE *input_file = fopen(argv[1], "r");

	if (!input_file) {
		std::cout << "error: input file does not exist" << std::endl;	
		exit(1);
	}

   //initializing manager
   WolValueFactory *valueFactory = new WolValueFactoryImpl();
   WolEvalFactory *evalFactory = new WolEvalFactoryImpl();
   WolMgr::getInstance().setWolValueFactory(valueFactory);
   WolMgr::getInstance().setWolEvalFactory(evalFactory);

	WolSmt2Parser *parser = new WolSmt2Parser(input_file);
   if (parser->parse())	
      parser->print();
   else 
      std::cout << "error while reading the smt file" <<endl;
   
   delete parser;
	return 0;
}

}
int main(int argc, char **argv) {
	wolver::wolver_main(argc, argv);

	return 0;
}

