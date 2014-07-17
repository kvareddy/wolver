#include  "wolexp.h"
#include "wolmgr.h"
#include <iostream>
#include <assert.h>
#include <string.h>
#include <memory>

namespace wolver {

using namespace std;

WolNode::WolNode(WolNodeType type, int precision) 
      : _type(type), _precision(precision)
{
   _arity = 0;
   _refCount = 1;
   // need to implement the singleton pattern for accessing manager.   
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







}

