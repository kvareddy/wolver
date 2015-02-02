#include "wolmgr.h"
#include "wolexp.h"
#include "wolvaluefactory.h"
#include "wolimplmgr.h"
#include "wolvarselmgr.h"
#include "wolvalue.h"
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <string>

namespace wolver
{

using namespace std;

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

	if (!initializeOutputNodes(outputsVec)) {
		std::cout << "Obvious unsatisfiability" << std::endl;
		return false;
	}
	nodeL inputs(inputsVec.begin(), inputsVec.end());
	nodeL outputs(outputsVec.begin(), outputsVec.end());

	// initial implication
	implResult res = WolImplMgr::getInstance().performImplications(inputs, outputs);
	if (res.first == false) {
		std::cout << "Initial Implication Failed" << std::endl;
		return false;
	}

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

		// insert a stack entry
		_backtrackStack.pushDecisionNode(pNode);
		pNode->setValue(pValue);

		res = WolImplMgr::getInstance().performImplications(pNode);
		if (res.first) {
			_backtrackStack.storeImplications(res.second);
			continue;
		}
		else {
			while (1) {
				WolNodeSptr currNode = _backtrackStack.getTopNode();
				WolValueSptr currValue = currNode->getValue();
				WolValueSptr adjustValue =
						WolImplMgr::getInstance().performGenarilization(currNode, currValue);
				WolValueSptr leftOut = _backtrackStack.backtrackOnCurrentVariable(adjustValue);
				if (leftOut) {
					currValue = leftOut->getRandomValue();
					currNode->setValue(currValue);
					res = WolImplMgr::getInstance().performImplications(currNode);
					if (res.first) {
						_backtrackStack.storeImplications(res.second);
						break;
					}
					else {
						continue;
					}
				}
				else {
					if (_backtrackStack.getStackSize() == 1) {
						std::cout << "The constraints are unsatisfiable" << std::endl;
						return false;
					}
					else {
						_backtrackStack.backtrackToNextVariable();
					}
				}
			}
		}

	}
	std::cout << "The constraints are unsatisfiable" << std::endl;
	return false;
}



}
