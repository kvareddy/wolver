/*
 * wolstack.h
 *
 *  Created on: Dec 10, 2014
 *      Author: user
 */

#ifndef WOLSTACK_H_
#define WOLSTACK_H_


#include "common.h"
#include <cassert>
#include <stack>

namespace wolver {


  class WolStackEntry {

  public:
    WolStackEntry(WolValueSptr value, WolNodeSptr node) :
      _node(node),
      _storedValue(value),
      _activeValue(value){}
    ~WolStackEntry() {
      assert(_implications.empty());
    }

  public:
    WolValueSptr getSotredValue() {return _storedValue;}
    WolValueSptr getActiveValue() {return _activeValue;}
    WolNodeSptr getNode() {return _node;}

    void populateImplications(nodeL &list);
    void updateActiveValue(WolValueSptr newValue) {_activeValue = newValue;}
    void restoreImplications();


  private:
    WolNodeSptr _node;
    WolValueSptr _storedValue;
    WolValueSptr _activeValue;
    nodeValueVec _implications;

  };


class WolStack {

public:
  WolStack(){}
  ~WolStack(){}

public:  // methods
  void pushDecisionNode(WolNodeSptr node);
  void storeImplications(nodeL &list);
  // backtrack on the top most variable on the stack
  // returns the left out range for the variable.
  WolValueSptr backtrackOnCurrentVariable(WolValueSptr adjustedValue);
  void backtrackToNextVariable();
  WolNodeSptr getTopNode() { return _stack.top().getNode();}
  unsigned getStackSize() {return _stack.size();}

private: // methods


private: // data
  std::stack<WolStackEntry> _stack;

};











}




#endif /* WOLSTACK_H_ */
