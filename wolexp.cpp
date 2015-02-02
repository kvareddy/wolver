#include "wolmgr.h"
#include "wolexp.h"
#include "wolevalfactory.h"
#include "wolvaluefactory.h"
#include "wolvalue.h"
#include "wolvaluefactory.h"
#include <iostream>
#include <assert.h>
#include <string.h>
#include <memory>

namespace wolver {

using namespace std;

WolNode::WolNode(WolNodeType type, int precision) 
      : _type(type),
        _precision(precision),
        _implyFlag(false),
        _svalue(nullptr)
{
   _arity = 0;
   _refCount = 1;
   _id = 0;
   _value = WolMgr::getInstance().getValueFactory()->makeComplexValue(precision);
}

void WolNode::addParent(WolNodeSptr node) {
    WolNodeWptr temp = node;
   _parents.push_back(temp);
}

void WolNode::deleteInvalidParents() {
      
   auto i = _parents.begin();
   while (i != _parents.end()) {
      if (i->expired()) {
         i = _parents.erase(i); 
         continue;
      }
      ++i;
   }
}

std::vector<WolNodeWptr> WolNode::getParents(){
   deleteInvalidParents();
   return _parents;
}

bool WolNode::wol_is_const_zero_or_ones_expr(){
   if(!wol_is_bv_const_node())
      return false;
         
   if ((_name[0] == '0' && (_name.find('1') == std::string::npos)) || 
       (_name[0] == '1' && (_name.find('0') == std::string::npos)))
      return true;

   return false;
}

bool WolNode::wol_is_const_one_expr() {
   
   if(!wol_is_bv_const_node())
      return false;

   if (_name[0] == '1' && (_name.find('1',1) == std::string::npos))
      return true;

   return false;
} 

std::vector<WolNodeSptr>
WolNode::getNeighbors() {
  std::vector<WolNodeSptr> retValue;
  deleteInvalidParents();
  for (auto i : _parents)
       retValue.push_back(i.lock());

  return retValue;
}

WolValueSptr
WolNode::performBackwardImplication(WolNodeSptr p,
                                    WolValueSptr parentValue,
                                    WolValueSptr operand1,
                                    WolValueSptr operand2) {

  WolEvalFactory *evalFactory = WolMgr::getInstance().getEvalFactory();
  WolValueFactory *valueFactory = WolMgr::getInstance().getValueFactory();
  WolValueSptr retValue = nullptr;

  WolComplexNodeSptr parent = dynamic_pointer_cast<WolComplexNode>(p);
  assert(parent);

  int parentArity = parent->getArity();
  WolNodeType parentType = parent->getType();
  assert(parentArity == 2 || parentArity == 3 || parentArity == 1);
  WolValueSptr tempResult = nullptr;
  if (parentArity == 2) {
    WolValueSptr operandValue = operand1;
    bool implyLeft = false;
    if(parent->getChild(0).get() == this ) {
      implyLeft = true;
    }
    switch(parentType) {
      case WOL_AND_NODE:
	tempResult = evalFactory->evalAndB(parentValue, operandValue);
	break;
      case WOL_BEQ_NODE:
	tempResult = evalFactory->evalBeqB(parentValue, operandValue);
	break;
      case WOL_ULT_NODE:
	tempResult = evalFactory->evalUltB(parentValue, operandValue, implyLeft);
	break;
      case WOL_CONCAT_NODE:
	tempResult = evalFactory->evalConcatB(parentValue, operandValue, implyLeft);
	break;
      default:
	assert(0);
    }
  }
  else if (parentArity == 3) {
    assert(parentType == WOL_BCOND_NODE);
    WolValueSptr operandValue1 = operand1;
    WolValueSptr operandValue2 = operand2;
    int position = -1;
    if(parent->getChild(0).get() == this ) {
      position = 0;
    }
    else if (parent->getChild(1).get() == this ) {
      position = 1;
    }
    else {
      position = 2;
    }
    tempResult = evalFactory->evalCondB(parentValue, operandValue1, operandValue2, position);
  }
  else {
    assert(parentType == WOL_SLICE_NODE ||parentType == WOL_BV_NOT_NODE);

    if (parentType == WOL_SLICE_NODE) {
    	tempResult = evalFactory->evalSpliceB(parentValue,
    										  parent->getHighPrecision(),
											  parent->getLowPrecision(),
											  _precision);
    }
    else {
    	tempResult = parent->getValue()->getNotValue();
    }
  }

  retValue = tempResult;
  return retValue;
}

WolValueSptr
WolNode::performBackwardImplication() {

	WolEvalFactory *evalFactory = WolMgr::getInstance().getEvalFactory();
	WolValueFactory *valueFactory = WolMgr::getInstance().getValueFactory();
	bool implPerformed = false;
	WolValueSptr retValue = nullptr;

	for( auto i : _parents) {
		WolComplexNodeSptr parent = dynamic_pointer_cast<WolComplexNode>(i.lock());
		assert(parent);
		if (!parent->getImplyFlag()) continue;

		WolValueSptr parentValue = parent->getValue();
		int parentArity = parent->getArity();
		WolNodeType parentType = parent->getType();
		assert(parentArity == 2 || parentArity == 3 || parentArity == 1);
		WolValueSptr tempResult = nullptr;
		if (parentArity == 2) {
			WolValueSptr operandValue = nullptr;
			bool implyLeft = false;
			if(parent->getChild(0).get() == this ) {
				operandValue = parent->getChild(1)->getValue();
				implyLeft = true;
			}
			else {
				operandValue = parent->getChild(0)->getValue();
			}

			switch(parentType) {
			case WOL_AND_NODE:
				tempResult = evalFactory->evalAndB(parentValue, operandValue);
				break;
			case WOL_BEQ_NODE:
				tempResult = evalFactory->evalBeqB(parentValue, operandValue);
				break;
			case WOL_ULT_NODE:
				tempResult = evalFactory->evalUltB(parentValue, operandValue, implyLeft);
				break;
			case WOL_CONCAT_NODE:
				tempResult = evalFactory->evalConcatB(parentValue, operandValue, implyLeft);
				break;
			default:
				assert(0);
			}
		}
		else if (parentArity == 3) {
			assert(parentType == WOL_BCOND_NODE);
			WolValueSptr operandValue1 = nullptr;
			WolValueSptr operandValue2 = nullptr;
			int position = -1;
			if(parent->getChild(0).get() == this ) {
				operandValue1 = parent->getChild(1)->getValue();
				operandValue2 = parent->getChild(2)->getValue();
				position = 0;
			}
			else if (parent->getChild(1).get() == this ) {
				operandValue1 = parent->getChild(0)->getValue();
				operandValue2 = parent->getChild(2)->getValue();
				position = 1;
			}
			else {
				operandValue1 = parent->getChild(0)->getValue();
				operandValue2 = parent->getChild(1)->getValue();
				position = 2;
			}
			tempResult = evalFactory->evalCondB(parentValue, operandValue1, operandValue2, position);
		}
		else {
			assert(parentType == WOL_SLICE_NODE || parentType == WOL_BV_NOT_NODE);

			if (parentType == WOL_SLICE_NODE) {
				tempResult = evalFactory->evalSpliceB(parentValue,
						parent->getHighPrecision(),
						parent->getLowPrecision(),
						_precision);
			}
			else {
				tempResult = parent->getValue()->getNotValue();
			}
		}

		if (!implPerformed) retValue = tempResult;
		else retValue = evalFactory->evalIntersection(retValue, tempResult);
		implPerformed = true;

	}

	if(implPerformed) return retValue;
	return valueFactory->makeComplexValue(_precision);

}

// result = -1; conflict
// result = 0 ; nothing happened
// result = 1 ; enable backward implication
// result = 2 ; enable both forward and backward implication
int
WolNode::performImplication() {

  WolValueSptr currValue = performBackwardImplication();
  if(!currValue) return -1;

  WolValueSptr prevValue = _value;
  WolEvalFactory *evalFactory = WolMgr::getInstance().getEvalFactory();

  WolValueSptr intersectionValue = evalFactory->evalIntersection(prevValue, currValue);
  if (!intersectionValue) return -1;

  WolValueSptr diff1 = evalFactory->evalDiff(prevValue, currValue);
  WolValueSptr diff2 = evalFactory->evalDiff(currValue, prevValue);

  if(!diff1 & !diff2) return 0;
  if (!diff1) return 1;

  if(!_svalue) _svalue = prevValue;
  _value = intersectionValue;

  return 2;
}

WolValueSptr
WolNode::getRandomValue() {

  return _value->getRandomValue();
}

WolComplexNode::WolComplexNode(WolNodeType type,
                               int precision, 
                               WolNodeSptr child) 
                : WolNode(type, precision), 
                  _highPrec(0),
                  _lowPrec(0)
{
   _children[0] = child;
   _arity = 1;
}

WolComplexNode::WolComplexNode(WolNodeType type,
                               int precision,
                               WolNodeSptr child1,
                               WolNodeSptr child2) 
                : WolNode(type, precision), 
                  _highPrec(0), 
                  _lowPrec(0)
{
   _children[0] = child1;
   _children[1] = child2;
   _arity = 2;
}

WolComplexNode::WolComplexNode(WolNodeType type, 
                               int precision, 
                               int high_prec, 
                               int low_prec,
                               WolNodeSptr child1) 
                : WolNode(type, precision), 
                  _highPrec(high_prec), 
                  _lowPrec(low_prec)
{
   _children[0] = child1;
   _arity = 1;
}

WolComplexNode::WolComplexNode(WolNodeType type, 
                               int precision,
                               WolNodeSptr child1, 
                               WolNodeSptr child2, 
                               WolNodeSptr child3)
               : WolNode(type, precision), 
                 _highPrec(0), 
                 _lowPrec(0)

{
   _children[0] = child1;
   _children[1] = child2;
   _children[2] = child3;
   _arity = 3;
}

WolComplexNode::~WolComplexNode() {
   
   for (int i = 0; i < _arity; i++) {
      _children[i].reset();
   }
}

void WolComplexNode::print() {

   cout << '(' << _name;
   for (int i = 0; i < _arity ; i++) {
         cout << " " ;
         _children[i]->print();
   }
   cout << ')';
   
}

void WolComplexNode::setChildren(WolNodeSptr child, int index) {
   assert(index >= 0);
   assert(index <= 2);
   _children[index] = child;
}

bool WolComplexNode::wol_is_xor_expr(){
   //checking fori the case:
   // 1. !(!a && !b) && !(a && b)
   // can be further improved
   if (!wol_is_and_node())
      return false;
   
   if (!(_children[0]->wol_is_not_node() && 
         _children[0]->getChild(0)->wol_is_and_node()))
      return false;
        
   if (!(_children[1]->wol_is_not_node() && 
         _children[1]->getChild(0)->wol_is_and_node()))
      return false;
  
   WolNodeSptr expr0_0 = getChild(0)->getChild(0)->getChild(0);
   WolNodeSptr expr0_1 = getChild(0)->getChild(0)->getChild(1);
   WolNodeSptr expr1_0 = getChild(1)->getChild(0)->getChild(0);
   WolNodeSptr expr1_1 = getChild(1)->getChild(0)->getChild(1);

   if ((expr0_0->wol_is_not_node() && (expr0_0->getChild(0) == expr1_0)) &&
       (expr0_1->wol_is_not_node() && (expr0_1->getChild(0) == expr1_1)))
      return true; 

  return false;  
}

bool WolComplexNode::wol_is_xnor_expr(){

   if (wol_is_not_node() && getChild(0)->wol_is_xor_expr())
      return true;
   
   return false;
}

std::vector<WolNodeSptr>
WolComplexNode::getNeighbors() {
  std::vector<WolNodeSptr> neighbors;
  deleteInvalidParents();
  for(auto i : _parents)
    neighbors.push_back(i.lock());
  for (int j =0; j < _arity; j++) {
	  neighbors.push_back(_children[j]);
  }
  return neighbors;
}

WolValueSptr
WolComplexNode::performForwardImplication() {

  WolEvalFactory *evalFactory = WolMgr::getInstance().getEvalFactory();
  WolValueSptr retValue = nullptr;
  switch (_type) {
    case WOL_SLICE_NODE:
      retValue = evalFactory->evalSplice(getChild(0)->getValue(), _highPrec, _lowPrec);
      break;
    case WOL_AND_NODE:
      retValue = evalFactory->evalAnd(getChild(0)->getValue(), getChild(1)->getValue());
      break;
    case WOL_BEQ_NODE:
      retValue = evalFactory->evalBeq(getChild(0)->getValue(), getChild(1)->getValue());
      break;
    case WOL_ADD_NODE:
      assert(0);
      break;
    case WOL_MUL_NODE:
      assert(0);
      break;
    case WOL_ULT_NODE:
      retValue = evalFactory->evalUlt(getChild(0)->getValue(), getChild(1)->getValue());
      break;
    case WOL_SLL_NODE:
      assert(0);
      break;
    case WOL_SRL_NODE:
      assert(0);
      break;
    case WOL_UDIV_NODE:
      assert(0);
      break;
    case WOL_UREM_NODE:
      assert(0);
      break;
    case WOL_CONCAT_NODE:
      retValue = evalFactory->evalConcat(getChild(0)->getValue(), getChild(1)->getValue());
      break;
    case WOL_BCOND_NODE:
      retValue = evalFactory->evalCond(getChild(0)->getValue(),
                                       getChild(1)->getValue(),
                                       getChild(2)->getValue());
      break;
    case WOL_BV_NOT_NODE:
      retValue = ((getChild(0))->getValue())->getNotValue();
      break;
    default:
      assert(0);
  }
  return retValue;
}

WolValueSptr
WolComplexNode::performForwardImplication(WolValueSptr operand1,
                                          WolValueSptr operand2,
                                          WolValueSptr operand3) {

  WolEvalFactory *evalFactory = WolMgr::getInstance().getEvalFactory();
  WolValueSptr retValue = nullptr;
  switch (_type) {
    case WOL_SLICE_NODE:
      retValue = evalFactory->evalSplice(operand1, _highPrec, _lowPrec);
      break;
    case WOL_AND_NODE:
      retValue = evalFactory->evalAnd(operand1, operand2);
      break;
    case WOL_BEQ_NODE:
      retValue = evalFactory->evalBeq(operand1, operand2);
      break;
    case WOL_ADD_NODE:
      assert(0);
      break;
    case WOL_MUL_NODE:
      assert(0);
      break;
    case WOL_ULT_NODE:
      retValue = evalFactory->evalUlt(operand1, operand2);
      break;
    case WOL_SLL_NODE:
      assert(0);
      break;
    case WOL_SRL_NODE:
      assert(0);
      break;
    case WOL_UDIV_NODE:
      assert(0);
      break;
    case WOL_UREM_NODE:
      assert(0);
      break;
    case WOL_CONCAT_NODE:
      retValue = evalFactory->evalConcat(operand1, operand2);
      break;
    case WOL_BCOND_NODE:
      retValue = evalFactory->evalCond(operand1,
                                       operand2,
                                       operand3);
      break;
    case WOL_BV_NOT_NODE:
      retValue = operand1->getNotValue();
      break;
    default:
      assert(0);
  }
  return retValue;
}

int
WolComplexNode::performImplication() {
  WolValueSptr prevValue = _value;
  WolEvalFactory *evalFactory = WolMgr::getInstance().getEvalFactory();

  WolValueSptr currBackValue = performBackwardImplication();
  if (!currBackValue) return -1;

  WolValueSptr currForValue = performForwardImplication();
  if(!currForValue) return -1;

  WolValueSptr currValue = evalFactory->evalIntersection(currBackValue, currForValue);
  if (!currValue) return -1;

  WolValueSptr intersectionValue = evalFactory->evalIntersection(prevValue, currValue);
  if (!intersectionValue) return -1;

  WolValueSptr diff1 = evalFactory->evalDiff(prevValue, currValue);
  WolValueSptr diff2 = evalFactory->evalDiff(currValue, prevValue);

  if(!diff1 && !diff2)
	  return 0;

  if (!diff1)
	  return 1;

  if(!_svalue) _svalue = prevValue;
  _value = intersectionValue;

  return 2;
}


}

