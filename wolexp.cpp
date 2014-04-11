#include  "wolexp.h"
#include "wolmgr.h"
#include <iostream>
#include <assert.h>
#include <string.h>

namespace wolver {

using namespace std;

WolNode::WolNode(WolNodeType type, int precision) 
      : _type(type), _precision(precision)
{
   _arity = 0;
   _refCount = 1;
   // need to implement the singleton pattern for accessing manager.   
}

void WolNode::addParent(WolNode *node) {
   _parents.push_back(node);

}

WolComplexNode::WolComplexNode(WolNodeType type, int precision, WolNode *child) 
                : WolNode(type, precision), _highPrec(0), _lowPrec(0)
{
   _children[0] = child;
   child->incRefCount();
   child->addParent(this);
   _arity = 1;
}

WolComplexNode::WolComplexNode(WolNodeType type, int precision, WolNode *child1, WolNode *child2) 
                : WolNode(type, precision), _highPrec(0), _lowPrec(0)
{
   _children[0] = child1;
   child1->incRefCount();
   child1->addParent(this);
   _children[1] = child2;
   child2->incRefCount();
   child2->addParent(this);
   _arity = 2;
}

WolComplexNode::WolComplexNode(WolNodeType type, int precision, int high_prec, 
                               int low_prec, WolNode *child1) 
                : WolNode(type, precision), _highPrec(high_prec), _lowPrec(low_prec)
{
   _children[0] = child1;
   child1->incRefCount(); 
   child1->addParent(this);
   _arity = 1;
}

WolComplexNode::WolComplexNode(WolNodeType type, int precision,
                  WolNode *child1, WolNode *child2, WolNode *child3)
               : WolNode(type, precision), _highPrec(0), _lowPrec(0)

{
   _children[0] = child1;
   child1->incRefCount(); 
   child1->addParent(this);
   _children[1] = child2;
   child2->incRefCount();
   child2->addParent(this);
   _children[2] = child3;
   child3->incRefCount();
   child3->addParent(this);
   _arity = 3;
}

void WolComplexNode::release() {
   _refCount--;
   
   if (_refCount <= 0) {
      for (int i = 0; i < _arity; i++) 
         _children[i]->release();
      delete this;
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

void WolComplexNode::setChildren(WolNode *child, int index) {
   assert(index >= 0);
   assert(index <= 2);
   _children[index] = child;
}

}

