#include "wolmgr.h"
#include "wolexp.h"
#include "wolvaluefactory.h"
#include "wolimplmgr.h"
#include "wolvarselmgr.h"
#include "wolvalue.h"
#include "wollog.h"
#include "common.h"
#include "wolstack.h"
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <string>

namespace wolver
{

using namespace std;

std::size_t
sliceExpHash::operator ()(WolNodeSptr const& x) const
{
	std::size_t seed = 0;
	WolComplexNodeSptr x_comp = dynamic_pointer_cast<WolComplexNode>(x);
	seed = (size_t)((x_comp->getChildren())[0]->getId() + x_comp->getHighPrecision() +
			x_comp->getLowPrecision());
	return seed;
}

bool
sliceExpEqualTo::operator() (WolNodeSptr const& x, WolNodeSptr const& y) const
{
	WolComplexNodeSptr x_comp = dynamic_pointer_cast<WolComplexNode>(x);
	WolComplexNodeSptr y_comp = dynamic_pointer_cast<WolComplexNode>(y);

	if(((x_comp->getChildren()[0]) != (y_comp->getChildren())[0]) ||
			(x_comp->getHighPrecision() != y_comp->getHighPrecision()) ||
			(y_comp->getLowPrecision() != y_comp->getLowPrecision()))
		return false;

	return true;
}

bool
expEqualTo::operator()(WolNodeSptr const& x, WolNodeSptr const& y) const
{
	WolComplexNodeSptr x_comp = dynamic_pointer_cast<WolComplexNode>(x);
	WolComplexNodeSptr y_comp = dynamic_pointer_cast<WolComplexNode>(y);

	if ((x_comp->getType() != y_comp->getType()) ||
			(x_comp->getArity() != y_comp->getArity()))
		return false;

	int arity = x_comp->getArity();
	for (int i = 0; i < arity; i++)
	{
		if ((x_comp->getChildren())[i] != (y_comp->getChildren())[i])
			return false;
	}
	return true;
}

std::size_t
expHash::operator()(WolNodeSptr const& x) const
{
	std::size_t seed = 0;
	WolComplexNodeSptr x_comp = dynamic_pointer_cast<WolComplexNode>(x);

	int arity = x_comp->getArity();
	for (int i = 0; i < arity; i++)
	{
		seed += (size_t)(x_comp->getChildren())[i]->getId();
	}
	return seed;
}


WolMgr::WolMgr()
       : _globalId(1),
       _valueFactory(NULL),
       _evalFactory(NULL)
{
	_backtrackStack = new WolStack();
}

WolMgr::~WolMgr()
{
	delete _backtrackStack;
}

WolNodeSptr
WolMgr::findSliceExpr (WolNodeSptr exp) {
	std::unordered_set<WolNodeSptr, sliceExpHash, sliceExpEqualTo>::iterator get =
			_uniqSliceExpTable.find (exp);
	if (get == _uniqSliceExpTable.end ())
		return NULL;
	return *get;
}

WolNodeSptr
WolMgr::findUniqueExpr (WolNodeSptr exp) {
	std::unordered_set<WolNodeSptr, expHash, expEqualTo>::iterator get =
		_uniqExpTable.find (exp);
	if (get == _uniqExpTable.end ())
		return NULL;
	return *get;
}

void
WolMgr::insertConstExpr(WolNodeSptr exp) {
	_uniqConstTable[exp->getName()] = exp;
}

WolNodeSptr
WolMgr::findConstExpr (std::string name) {
	std::unordered_map<std::string, WolNodeSptr>::iterator get =
			_uniqConstTable.find (name);
	if (get == _uniqConstTable.end ())
		return NULL;
	return get->second;
}

void
WolMgr::insertIdExpr (WolNodeSptr exp) {
	_globalId++;
	_idToExpMap[_globalId] = exp;
	exp->setId (_globalId);
}

WolNodeSptr
WolMgr::findExpr (int id) {
	std::unordered_map<int, WolNodeSptr>::iterator get = _idToExpMap.find (id);
	if (get == _idToExpMap.end ())
		return NULL;
	return get->second;
}

bool
WolMgr::initializeOutputNodes(nodeVec outputs) {

	for (auto i : outputs) {
		auto one = _valueFactory->makeConstValue("1");

		if (!i->getValue()->containsOne())
			return false;
		i->setValue(one);
	}
	return true;
}

void
WolMgr::setOutputNodes(nodeVec outputsVec) {
	_outputs = outputsVec;
}

void
WolMgr::printgv () {

	std::ofstream fs;
	fs.open("output.gv");
	int numOutputs = _outputs.size ();
	fs << "digraph G {" << std::endl;
	for (int i = 0; i < numOutputs; i++) {
			_outputs[i]->unsetPrintFlag();
	}
	for (int i = 0; i < numOutputs; i++) {
		_outputs[i]->printgv(fs);
	}
	fs << "}" << std::endl;
    fs.close();

}

/*
 * assign output nodes to true
 * create input and output sets
 * initial implication
 * loop {
 * 	pick a variable
 *    // if there are no variable left return sat
 * 	pick a value for the variable
 * 	insert a stack entry
 * 	perform implication
 * 	if (!conflict)
 * 		store changes in stack
 * 		continue;
 * 	else
 * 		loop {
 * 			perform conflict generalization (top entry in stack)
 * 			perform backtrack on variable
 * 		        if (successful)
 * 		        	pick a new value for the variable
 * 		        	perform implication
 * 		        	if (!conflict)
 * 		        	        store changes in stack
 * 		        		break;
 * 		        	else
 * 		        		continue;
 *
 * 		        else
 * 		            if stack is not empty
 * 		        	backtrack to next variable
 * 		            else return unsat
 * 		}
 * }
 */
bool
WolMgr::solve(nodeVec inputsVec, nodeVec outputsVec) {

	DEBUG_MSG << "Initializing Output Nodes";
	if (!initializeOutputNodes(outputsVec)) {
		std::cout << "Obvious unsatisfiability" << std::endl;
		return false;
	}
	nodeL inputs(inputsVec.begin(), inputsVec.end());
	nodeL outputs(outputsVec.begin(), outputsVec.end());

	// initial implication
	DEBUG_MSG << "Begin Initial Implication";
	implResult res = WolImplMgr::getInstance().performImplications(inputs, outputs);
	if (res.first == false) {
		std::cout << "Initial Implication Failed" << std::endl;
		return false;
	}
    DEBUG_MSG << "End Initial Implication";


	while(1) {

		//pick a variable
		WolNodeSptr pNode = WolVarSelMgr::getInstance().pickNextVariable();
		if (!pNode) {
			for (auto i : inputs) {
				std::cout << i->getName() << " has Value " << i->getValue()->getStringRep() << std::endl;
			}
			return true;
		}
		// pick a value for the variable
		WolValueSptr pValue = pNode->getRandomValue();
		DEBUG_MSG << "picked variable " << pNode->getName()
				                        << " with value assignment "
										<< pValue->getStringRep();

		// insert a stack entry
		_backtrackStack->pushDecisionNode(pNode);
		pNode->setValue(pValue);

		res = WolImplMgr::getInstance().performImplications(pNode);
		if (res.first) {
			DEBUG_MSG << "Implication successful for variable assignment";
			_backtrackStack->storeImplications(res.second);
			continue;
		}
		else {
			DEBUG_MSG << "Implication failed for variable assignment";
			while (1) {
				WolNodeSptr currNode = _backtrackStack->getTopNode();
				WolValueSptr currValue = currNode->getValue();
				WolValueSptr adjustValue =
						WolImplMgr::getInstance().performGenarilization(currNode, currValue);
				WolValueSptr leftOut = _backtrackStack->backtrackOnCurrentVariable(adjustValue);
				if (leftOut) {
					currValue = leftOut->getRandomValue();
					currNode->setValue(currValue);

					DEBUG_MSG << "Backtracking to variable "
							 << currNode->getName()
							 << " with new assignment value "
							 << currValue->getStringRep();


					res = WolImplMgr::getInstance().performImplications(currNode);
					if (res.first) {
						_backtrackStack->storeImplications(res.second);
						break;
					}
					else {
						continue;
					}
				}
				else {
					DEBUG_MSG << "Backtracking failed on variable "
							 << currNode->getName();
					if (_backtrackStack->getStackSize() == 1) {
						std::cout << "The constraints are unsatisfiable" << std::endl;
						return false;
					}
					else {
						_backtrackStack->backtrackToNextVariable();
					}
				}
			}
		}

	}
	std::cout << "The constraints are unsatisfiable" << std::endl;
	return false;
}



}
