#include <iostream>
#include <stdio.h>
#include <vector>
#include "wolmgr.h"
#include "wolexp.h"
#include "wolsmt2.h"
#include "wolexpfactory.h"
#include "wolevalfactory.h"
#include "wolvaluefactory.h"
#include "wolvaluefactoryimpl.h"
#include "wolevalfactoryimpl.h"
#include "wolvarselmgr.h"
#include "wollog.h"
//#define CATCH_CONFIG_RUNNER
//#include "catch.hpp"

namespace wolver {

int wolver_main(FILE* input_file, FILE* output_file)
{
	//initializing manager
	WolValueFactory *valueFactory = new WolValueFactoryImpl();
	WolEvalFactory *evalFactory = new WolEvalFactoryImpl();
	WolMgr::getInstance().setWolValueFactory(valueFactory);
	WolMgr::getInstance().setWolEvalFactory(evalFactory);

	DEBUG_MSG << "Begin Parsing";
	WolSmt2Parser *parser = new WolSmt2Parser(input_file);
	if (parser->parse()) {
		parser->print();
		parser->printgv();
	}
	else
		std::cout << "error while reading the smt file" <<endl;
	//TODO: Display routine
	DEBUG_MSG << "End Parsing";


	WolVarSelMgr::getInstance().setStrategy(WolVarSelMgr::Random,
			parser->getInputs());

	WolMgr::getInstance().setOutputNodes(parser->getOutputs());
	DEBUG_MSG << "Start Solving";
	bool result = WolMgr::getInstance().solve(parser->getInputs(),
			parser->getOutputs());
	DEBUG_MSG << "End Solving";

	// dumping output stream to an output file.
	std::ostringstream& os = WolMgr::getInstance().getOutputStream();
	fprintf(output_file, "%s", os.str().c_str());
	fflush(output_file);
	os.str().clear();


	delete parser;
	return true;
}

}
int main(int argc, char **argv) {

	//	TODO: option infrastructure
	FILE *input_file = fopen(argv[1], "r");
	if (!input_file) {
		std::cout << "error: input file does not exist" << std::endl;
		exit(1);
	}

	// Initializing logging
	WolLog::ReportingLevel() = WolLog::FromString(argc == 3 ? argv[2] : "DEBUG4");
	if (argc == 4)  // log file
		WolLog::Stream() = fopen(argv[3], "w");

	wolver::wolver_main(input_file, stdout);
	fclose(input_file);

	// Unit Testing
	//int result = Catch::Session().run();

	return 0;
}




